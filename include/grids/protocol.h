
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

        void send_request(const std::string& event_type,
            bool broadcast = true);
        void send_request(const std::string& event_type, 
            Value&, bool broadcast = true);

        bool socket_connected();

    private:
        void handle_message(const std::string&);
        Value parse_json(const std::string&);
        size_t protocol_write(const std::string&);
        size_t protocol_write(const char*, size_t len);
        void send_protocol_initialization_string();
        std::string stringify_value(const Value&);

        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::resolver* resolver_;
        boost::asio::ip::tcp::resolver::query* query_;
        boost::asio::ip::tcp::resolver::iterator iterator_;
        boost::asio::ip::tcp::socket* socket_;

        Json::FastWriter* writer_;

        static const std::string method_key_;
        static const std::string broadcast_key_;
    };
}
