
#pragma once

#include <boost/asio.hpp>

#include <grids/define.h>

namespace Grids {
    class Protocol {
    public:
        Protocol();
        ~Protocol();
        void init();

        enum { GRIDS_PORT = 1488 };

        bool connect_to_node(const std::string& address);

		void send_request(Value&); // Use this most often
        void send_request(const std::string& event_type,
            bool broadcast = true); // Use this for simple requests like LIST_ROOMS

        void send_request(const std::string& event_type, 
            Value&, bool broadcast = true); // Old, sholudn't be used

        bool socket_connected();

        void check_network();

		static const char* method_key;
        static const char* broadcast_key;

    private:
        void handle_message(const std::string&);
        size_t protocol_write(const std::string&);
        size_t protocol_write(const char*, size_t len);
        void send_protocol_initialization_string();
        std::string stringify_value(const Value&);
        void grids_read();
        void protocol_initiated();
        void protocol_initiated_encrypted();

        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::resolver* resolver_;
        boost::asio::ip::tcp::resolver::query* query_;
        boost::asio::ip::tcp::resolver::iterator iterator_;
        boost::asio::ip::tcp::socket* socket_;

        Json::FastWriter* writer_;
		Json::Reader* reader_;

        static const bool debug_print_ = true;

        bool protocol_initiated_;
        bool protocol_initiated_encrypted_;
    };
}
