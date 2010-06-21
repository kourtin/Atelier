
#include <json/json.h>

#include <gridsNetworkItem.h>
#include <identity.h>
#include <grids/interface.h>
#include <grids/protocol.h>
#include <client.h>
#include <basicApp.h>

namespace Atelier {
    GridsNetworkItem::~GridsNetworkItem() {
    }
    
    void GridsNetworkItem::activate(const Identity& ident) {
        Value val;

        Client::grids_interface().protocol().send_request(
            Grids::Interface::GRIDS_LIST_ROOMS, val);
    }


}