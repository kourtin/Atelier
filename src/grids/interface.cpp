
#include <cinder/app/App.h>

#include <grids/interface.h>
#include <grids/protocol.h>
#include <define.h>
#include <grids/define.h>
#include <tete.h>
#include <identity.h>

namespace Grids {
    const char* Interface::server_address_ = "mmmii.net";

    const char* Interface::GRIDS_CREATE_ROOM = "Room.Create"; 
    const char* Interface::GRIDS_CREATE_OBJECT = "Room.CreateObject";
    const char* Interface::GRIDS_UPDATE_OBJECT = "Room.UpdateObject";
    const char* Interface::GRIDS_LIST_ROOMS = "Room.List";
    const char* Interface::GRIDS_NULL_EVENT = "NULL_EVENT";

    Interface::Interface() {
        instance_ = this;
    }

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

    void Interface::update() {
        protocol_->check_network();
    }

    Atelier::Tete* Interface::parse_network_event(Value& val) {
        // This is the id of the tete (of the Object itself), not the creator / owner
        Atelier::ID tete_id = val["id"].asString();

        int num_links = 0;
        std::vector<const Atelier::Identity*> link_identities;

        if (!val["attr"]["links"].empty()) {
            Value& links = val["attr"]["links"];

            for (Value::iterator it = links.begin(); it != links.end(); ++it) {
                const Atelier::Identity* ident = Atelier::Identity::get_identity_from_value(*it);
                if (ident == NULL)
                    ident = Atelier::Identity::create_identity(*it);

                link_identities.push_back(ident);
            }
        }

        //const Atelier::Identity* tete_ident = Atelier::Identity::get_identity_from_id(tete_id);
        // If NULL, then we've never heard of the creator of this Tete before, so
        // create it, and fill in details now or later.
        // Note: every Atelier::Object in existance should have an Identity, so if 
        // we can't find an identity, the object doesn't exist (yet?), and is probably
        // from the network.

        // This will copy the value, so it can be deleted
        Atelier::Tete* tete = new Atelier::Tete(link_identities, val);

        return tete;
    }

	void Interface::request_list_rooms() {
		protocol_->send_request(GRIDS_LIST_ROOMS, false);
	}

	void Interface::request_create_room() {
		protocol_->send_request(GRIDS_CREATE_ROOM, true);
	}

	// NOTE: This method is probably not needed anymone. It is implemented in Protocol
	// probably should be deleted after being added to version control
	void Interface::set_value_from_type(Atelier::Tete& tete, Atelier::Tete::Type ty) {
		switch(ty) {
		case Atelier::Tete::INVALID:
			return;
			break;
		case Atelier::Tete::CREATE_ROOM:
			tete.value()[Grids::Protocol::method_key] = GRIDS_CREATE_ROOM;
			break;
		case Atelier::Tete::LIST_ROOMS:
			tete.value()[Grids::Protocol::method_key] = GRIDS_LIST_ROOMS;
			break;
		case Atelier::Tete::CREATE:
			tete.value()[Grids::Protocol::method_key] = GRIDS_CREATE_OBJECT;
			break;
		case Atelier::Tete::UPDATE:
			tete.value()[Grids::Protocol::method_key] = GRIDS_UPDATE_OBJECT;
			break;
		}
	}

	void Interface::set_value_from_broadcast(Atelier::Tete& tete, bool broadcast) {
		tete.value()[Grids::Protocol::broadcast_key] = broadcast;
	}

	void Interface::send_tete(Atelier::Tete& tete) {
		set_value_links(tete.value(), tete);

		protocol_->send_request(tete.value());
	}

	// Converts from Tete identities to links
	void Interface::set_value_links(Value& val, Atelier::Tete& tete) {
		std::vector<const Atelier::Identity*> links = tete.links();

		for (std::vector<const Atelier::Identity*>::iterator it = links.begin();
			it != links.end(); ++it) {
			Value link_value;
			link_value["id"] = (*it)->id();
			link_value["name"] = (*it)->name();

			val["attr"]["links"].append(link_value);
		}
	}
}
