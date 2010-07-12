
#include <list>

#include <cinder/app/App.h>

#include <grids/interface.h>
#include <grids/protocol.h>
#include <define.h>
#include <grids/define.h>
#include <tete.h>
#include <identity.h>
#include <link.h>
#include <utility.h>

namespace Grids {
    const char* Interface::server_address_ = "mmmii.net";

    const char* Interface::GRIDS_CREATE_ROOM = "Room.Create"; 
    const char* Interface::GRIDS_CREATE_OBJECT = "Room.CreateObject";
    const char* Interface::GRIDS_UPDATE_OBJECT = "Room.UpdateObject";
    const char* Interface::GRIDS_LIST_ROOMS = "Room.List";
    const char* Interface::GRIDS_NULL_EVENT = "NULL_EVENT";

    Interface::Interface() {
        instance_ = this;
		reject_confirmation_ = true;
		local_room_ = Atelier::Utility::create_uuid();
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
		// TODO: FIX THIS IF STATEMENT!!!!!!!!!!!!!!!!!!!!!!!
		if (reject_confirmation_ && !val["success"].empty() && 
			val[Protocol::method_key].asString() != GRIDS_LIST_ROOMS && 
			val[Protocol::method_key].asString() != GRIDS_CREATE_ROOM)
			return NULL;
		
		// This is the id of the tete (of the Object itself), not the creator / owner
        Atelier::ID tete_id = val["id"].asString();

        int num_links = 0;
        Atelier::LinkList link_list;

        // Note: I think this is the problem:
        // The program receives a CreateObject event, but is linked to itself
        // therefore creating the object is sort of deligated to the 
        // get_identity... method. ??

        if (!val["attr"]["links"].empty()) {
            Value& links = val["attr"]["links"];

            for (Value::iterator it = links.begin(); it != links.end(); ++it) {
				const Atelier::Link* link = Atelier::Link::get_link_from_value(*it); 

                if (link != NULL)
                    link_list.push_back(link);
            }
        }

        //const Atelier::Identity* tete_ident = Atelier::Identity::get_identity_from_id(tete_id);
        // If NULL, then we've never heard of the creator of this Tete before, so
        // create it, and fill in details now or later.
        // Note: every Atelier::Object in existance should have an Identity, so if 
        // we can't find an identity, the object doesn't exist (yet?), and is probably
        // from the network.
		
        // This will copy the value, so it can be deleted
        Atelier::Tete* tete = Atelier::Tete::create_tete(link_list, val);

        return tete;
    }

	// Is there a way to make these valid of you're not connected to the network?
	void Interface::request_list_rooms() {
		if (protocol_->protocol_initiated())
			protocol_->send_request(GRIDS_LIST_ROOMS, false);
		else {
			Value val = generate_local_room();
            ci::app::console() << "WARNING: Not connected to network, using local room: " <<
                val["rooms"][0u].asString() << std::endl;
			parse_network_event(val);
            
		}
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

		send_request(tete);
	}

	void Interface::send_request(Atelier::Tete& tete) {
		if (protocol_->protocol_initiated())
			protocol_->send_request(tete.value());
		else {
			// Make a copy so we don't fuck with local data
			Value val = tete.value();
			parse_network_event(val);
		}
	}

	// Converts from Tete identities to links
	void Interface::set_value_links(Value& val, Atelier::Tete& tete) {
		Atelier::LinkList links = tete.links();

		for (Atelier::LinkList::iterator it = links.begin();
			it != links.end(); ++it) {
			Value link_value;
			link_value["id"] = (*it)->actor().id();
			link_value["name"] = (*it)->actor().name();
            link_value["read"] = (*it)->flags().can_read;
            link_value["modify"] = (*it)->flags().can_modify;
            link_value["creator"] = (*it)->flags().creator;
            link_value["remove"] = (*it)->flags().remove;

			val["attr"]["links"].append(link_value);
		}
	}

	// A dummy method used when the interface isn't connected to a node
	Value Interface::generate_local_room() {
		Value val;
		val[Protocol::method_key] = GRIDS_LIST_ROOMS;
		val["rooms"][0u] = local_room_;

		return val;
	}
}
