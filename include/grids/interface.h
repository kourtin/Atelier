
#pragma once

#include <string>

namespace Grids {
    class Protocol;

    class Interface {
    public:
        Interface();

        void init();
        void connect_to_node();

        bool connected();
        static Interface& instance();

        static Protocol& protocol();

        static const std::string GRIDS_CREATE_ROOM;
        static const std::string GRIDS_CREATE_OBJECT;
        static const std::string GRIDS_UPDATE_OBJECT;
        static const std::string GRIDS_LIST_ROOMS;
        static const std::string GRIDS_NULL_EVENT;

    private:
        static Protocol* protocol_;
        bool connected_;
        static Interface* instance_;

        static const std::string server_address_;
    };
}