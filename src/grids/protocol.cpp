
#include <sstream>
#include <exception>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <json/reader.h>
#include <json/writer.h>

#include <cinder/app/App.h>

#include <grids/protocol.h>
#include <client.h>

namespace Grids {
    const std::string Protocol::method_key_ = "_method";
    const std::string Protocol::broadcast_key_ = "_broadcast";

    Protocol::Protocol() {
    }

    void Protocol::init() {
        resolver_ = new boost::asio::ip::tcp::resolver(io_service_);
        writer_ = new Json::FastWriter();
    }

    Protocol::~Protocol() {
        // Make sure to unbind from any sockets
        socket_->close();

        delete resolver_;
        delete query_;
        delete socket_;
        delete writer_;
    }

    bool Protocol::connect_to_node(const std::string& address) {
        std::string port;
        std::stringstream port_conversion;
        port_conversion << GRIDS_PORT;
        port = port_conversion.str();

        try {
            query_ = new boost::asio::ip::tcp::resolver::query(
                boost::asio::ip::tcp::v4(), address, port);
        } catch (const std::exception& /*ec*/) {
            ci::app::console() << "Could not bind address or port" << std::endl;
            return false;
        }

        iterator_ = resolver_->resolve(*query_);
        socket_ = new boost::asio::ip::tcp::socket(io_service_);
        
        try {
            socket_->connect(*iterator_);
        } catch (const std::exception& /*ec*/) {
            ci::app::console() << "Could not connect to server" << std::endl;
            return false;
        }

        boost::asio::io_service io;
        boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));

        for(;;) {
            if(socket_->is_open())
                break;
            ci::app::console() << "Waiting to connect" << std::endl;
            t.wait();
            // maybe return false?
        }

        // TODO: check that we are connected
        // hooray we are connnected! initialize protocol
        send_protocol_initialization_string();

        return true;
    }

    void Protocol::send_protocol_initialization_string() {
        std::string init_string = "++Grids/1.0/JSON/name=\"";
        init_string += Atelier::Client::user_identity().id();
        init_string += "\"";
        protocol_write(init_string);
    }

    void Protocol::send_request(const std::string& event_type, 
        Value& args, bool broadcast) {
        if (event_type.empty())
            return;

        args[method_key_] = event_type;
        args[broadcast_key_] = broadcast;

        protocol_write(stringify_value(args));
    }

    std::string Protocol::stringify_value(const Value& val) {
        return writer_->write(val);
    }

    size_t Protocol::protocol_write(const std::string& str) {
        size_t len = str.size();

        return protocol_write(str.c_str(), len); 
    }

    // Note this cannot be threaded without updating send_request(const Value&,...)
    size_t Protocol::protocol_write(const char* str, size_t len) {
        if (!socket_connected())
            return 0;
        
        size_t ret;
        __int32 net_len = len;

        size_t outstr_len = len + 4;
        char* outstr = (char*)malloc(outstr_len);

        memcpy(outstr, &net_len, 4);
        //memcpy((outstr + 4), str, len);
        memcpy(&outstr[4], str, len);

        //ci::app::console() << std::string(&outstr[4]) << std::endl;

        ret = boost::asio::write(*socket_, boost::asio::buffer(outstr, outstr_len));
        free(outstr);

        return ret;
    }

    Value Protocol::parse_json(const std::string& msg) {
        Grids::Value root;
        Json::Reader reader;

        if (reader.parse(msg, root))
            return root;

        ci::app::console() << "Could not parse JSON: " << 
            msg << std::endl;

        return Value();
    }

    bool Protocol::socket_connected() {
        return (socket != NULL) && socket_->is_open();
    }
}