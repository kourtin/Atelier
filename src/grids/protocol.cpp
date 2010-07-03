
#include <sstream>
#include <exception>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/cstdint.hpp> // for uint32_t

#include <json/reader.h>
#include <json/writer.h>

#include <cinder/app/App.h>

#include <grids/protocol.h>
#include <client.h>
#include <utility.h>

namespace Grids {
    const char* Protocol::method_key = "_method";
    const char* Protocol::broadcast_key = "_broadcast";

    Protocol::Protocol() {
        resolver_ = NULL;
        query_ = NULL;
        socket_ = NULL;
        writer_ = NULL;
		protocol_initiated_ = false;
    }

    void Protocol::init() {
        resolver_ = new boost::asio::ip::tcp::resolver(io_service_);
        writer_ = new Json::FastWriter();
		reader_ = new Json::Reader();
    }

    Protocol::~Protocol() {
        // Make sure to unbind from any sockets
        socket_->close();

        delete resolver_;
        delete query_;
        delete socket_;
        delete writer_;
		delete reader_;
    }

    bool Protocol::connect_to_node(const std::string& address) {
        std::string port;
        std::stringstream port_conversion;
        port_conversion << GRIDS_PORT;
        port = port_conversion.str();

        try {
            query_ = new boost::asio::ip::tcp::resolver::query(
                boost::asio::ip::tcp::v4(), address, port);

            iterator_ = resolver_->resolve(*query_);
            socket_ = new boost::asio::ip::tcp::socket(io_service_);

            socket_->connect(*iterator_);
        } catch (const std::exception& ec) {
            ci::app::console() << "Exception: " << ec.what() << std::endl;
            ci::app::console() << "Could not connect to server" << std::endl;
            return false;
        }

        boost::asio::io_service io;
        boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));

        for (;;) {
            if(socket_->is_open())
                break;
            ci::app::console() << "Waiting to connect" << std::endl;
            t.wait();
            // maybe return false after a certain time?
        }

        // hooray we are connnected! initialize protocol
        send_protocol_initialization_string();
        
        protocol_initiated_ = true;

        return true;
    }

    void Protocol::send_protocol_initialization_string() {
        std::string init_string = "++Grids/1.0/JSON/name=\"";
        init_string += Atelier::Client::user_identity().id();
        init_string += "\"";
        protocol_write(init_string);
    }

    void Protocol::send_request(const std::string& event_type,
        bool broadcast) {
        Value val;
        send_request(event_type, val, broadcast);
    }

    void Protocol::send_request(const std::string& event_type, 
        Value& args, bool broadcast) {
        if (event_type.empty())
            return;

        args[method_key] = event_type;
        args[broadcast_key] = broadcast;

        protocol_write(stringify_value(args));
    }

	void Protocol::send_request(Value& args) {
        if (args[method_key].empty())
            return;

        protocol_write(stringify_value(args));
    }

    std::string Protocol::stringify_value(const Value& val) {
        return writer_->write(val);
    }

    size_t Protocol::protocol_write(const std::string& str) {
        size_t len = str.size();

        if (debug_print_) {
            ci::app::console() << "Sending:" << std::endl <<
                str << std::endl;
        }

        return protocol_write(str.c_str(), len); 
    }

    // Note this cannot be threaded without updating send_request(const Value&,...)
    size_t Protocol::protocol_write(const char* str, size_t len) {
        if (!socket_connected())
            return 0;
        
        size_t ret = 0u;
        boost::uint32_t net_len =len;
        Atelier::Utility::host_to_network_uint32(net_len);

        size_t outstr_len = len + 4;
        char* outstr = (char*)malloc(outstr_len);

        memcpy(outstr, &net_len, 4);
        //memcpy((outstr + 4), str, len);
        memcpy(&outstr[4], str, len);

        try {
            ret = boost::asio::write(*socket_, boost::asio::buffer(outstr, outstr_len));
        } catch (std::exception ex) {
            ci::app::console() << "Could not write to node" << std::endl <<
                ex.what() << std::endl;
        }

        free(outstr);

        return ret;
    }

    void Protocol::check_network() {
		if (protocol_initiated_ == false || socket_ == NULL)
			return;

		try {
			if (socket_->available() > 0u)
				grids_read();
		} catch (boost::system::system_error er) {
			// Probably not connected to network, abort and force reconnect
			ci::app::console() << "Error reading from network: " <<
				std::endl << er.what() << std::endl;
			protocol_initiated_ = false;
		}
    }

    void Protocol::grids_read() {
        size_t bytes_read;
        boost::uint32_t incoming_length;
        char* buf;
        char* buf_incoming;

        // Wait for more data if we don't have the length
        if(socket_->available() < 4)
            return;

        bytes_read = socket_->read_some(
            boost::asio::buffer((char*)(&incoming_length), 4));

        Atelier::Utility::network_to_host_uint32(incoming_length);

        if (bytes_read <= 0 || bytes_read != 4) {
            ci::app::console() << "Socket read error" << std::endl;
            return;
        }

        if (incoming_length > 1024 * 1024 * 1024) {
            // not going to read in more than a gig, f that
            std::cerr << "Got incoming message size: " << 
                incoming_length << ". Skipping\n";
            return;
        }

        // allocate space for incoming message + null byte
        buf = (char*)malloc(incoming_length + 1);
        boost::uint32_t bytes_remaining = incoming_length;
        buf_incoming = buf;

        do {
            bytes_read = socket_->read_some(
                boost::asio::buffer(buf_incoming, bytes_remaining));

            if (bytes_read > 0) {
                bytes_remaining -= bytes_read;
                buf_incoming += bytes_read;
            }

        } while ((bytes_read > 0) && bytes_remaining);
        
        buf[incoming_length] = '\0';

        if (bytes_read == -1) {
            // o snap read error
            ci::app::console() << "Socket read error: " << bytes_read << "\n";
            free(buf);
            return;
        }

        if (bytes_read == 0) {
            // not chill
            ci::app::console() << "Didn't read any data when expecting message of " << incoming_length << " bytes\n";
            free(buf);
            return;
        }

        if (bytes_read != incoming_length) {
            ci::app::console() << "Only read " << bytes_read << " bytes when expecting message of "
                << incoming_length << " bytes\n";
            free(buf);
            return;
        }

         std::string msg = std::string(buf);

        handle_message(msg);

        free(buf);
    }

    void Protocol::handle_message(const std::string& msg) {
        if (msg.size() < 2) return; // obv. bogus

        if (msg.find("==", 0, 2) == 0) {
            // protocol initiation message
           ci::app::console() << "Grids session initiated" << 
               std::endl;
            protocol_initiated_ = true;
        } else if (msg.find("--", 0, 2) == 0) {
            // encrypted protocol message
            protocol_initiated_encrypted();
        } else {
            // assume this is json for now
            Value root;

			if (reader_->parse(msg, root) == false) {
				ci::app::console() << "Unable to parse message" << std::endl;
				return;
			}

            if (debug_print_) {
                ci::app::console() << "Received:" << std::endl <<
                    root.toStyledString() << std::endl;
            }

            // This will copy the value passed to it into an Atelier::Tete,
            // so it's OK if root is deleted as it goes out of scope.
            Interface::instance().parse_network_event(root);
        }
    }

    bool Protocol::protocol_initiated() {
        return protocol_initiated_;
    }

    void Protocol::protocol_initiated_encrypted() {
        protocol_initiated_encrypted_ = true;
    }

    bool Protocol::socket_connected() {
        return (socket_ != NULL) && socket_->is_open();
    }
}