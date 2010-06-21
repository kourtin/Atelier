
#include <grids/interface.h>
#include <grids/protocol.h>

namespace Grids {
    const std::string Interface::server_address_ = "mmmii.net";

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

    Interface* Interface::instance_;

    Interface& Interface::instance() {
        return *instance_;
    }
}