
#pragma once

#include <string>

namespace Grids {
    class Protocol;

    class Interface {
    public:

        void init();
        void connect_to_node();

        bool connected();
        Interface& instance();

    private:
        Protocol* protocol_;
        bool connected_;
        static Interface* instance_;

        static const std::string server_address_;
    };
}