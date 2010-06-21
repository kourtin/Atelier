
#include <grids/interface.h>
#include <grids/protocol.h>

namespace Grids {
    const std::string Interface::server_address_ = "mmmii.net";

    const std::string Interface::GRIDS_CREATE_ROOM = "Room.Create";
    const std::string Interface::GRIDS_CREATE_OBJECT = "Room.CreateObject";
    const std::string Interface::GRIDS_UPDATE_OBJECT = "Room.UpdateObject";
    const std::string Interface::GRIDS_LIST_ROOMS = "Room.List";
    const std::string Interface::GRIDS_NULL_EVENT = "NULL_EVENT";

    void Interface::init() {
        protocol_ = new Protocol();
        protocol_->init();
    }

    void Interface::connect_to_node() {
        connected_ = protocol_->connect_to_node(server_address_);
    }

    bool Interface::connected() {
        return connected_;
    }

    Protocol* Interface::protocol_;
    Interface* Interface::instance_;

    Protocol& Interface::protocol() {
        return *protocol_;
    }

    Interface& Interface::instance() {
        return *instance_;
    }
}