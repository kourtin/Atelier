
#include <clientNode.h>
#include <tete.h>
#include <client.h>
#include <userNode.h>
#include <cameraNode.h>
#include <link.h>
#include <teteManager.h>
#include <objectCreator.h>
#include <grids/interface.h>
#include <clientUserNode.h>
#include <identity.h>

namespace Atelier {
	ClientNode::ClientNode(const ID& new_id) : Object(new_id) {
		initializing_ = true;
	}

	ClientNode::~ClientNode() {
	}

	void ClientNode::receive_tete(const Tete& tete) {
		//ci::app::console() << "ClientNode receiving tete:" << std::endl <<
		//	tete.value().toStyledString() << std::endl;
		
		const Identity* creator = Tete::get_creator(tete);

		if (!initializing_ || creator == NULL ||
			*creator != Client::user_identity()) {
			// Let the object creator figure shit out
			if (tete.type() == Tete::LIST_ROOMS)
				parse_list_rooms(tete);
			else if (tete.type() == Tete::CREATE_ROOM)
				parse_create_room(tete);
			else
				ObjectCreator::instance().receive_tete(tete); 
		} else { // Then it's probably some initialization bullshit
			if (tete.type() == Tete::CREATE)
				parse_create_tete(tete);
		}
	}

    const ID& ClientNode::id() const {
        return Object::id();
    }

    std::list<LinkConstPtr>& ClientNode::links() {
        return Object::links();
    }

	const std::list<LinkConstPtr>& ClientNode::links() const {
        return Object::links();
    }

    Vec3D ClientNode::position() const {
        return Object::position();
    }

    Vec3D ClientNode::rotation() const {
        return Object::rotation();
    }

    Vec3D ClientNode::scale() const {
        return Object::scale();
    }

    void ClientNode::set_position(Vec3D vec) {
        Object::set_position(vec);
    }

    void ClientNode::set_rotation(Vec3D vec) {
        Object::set_rotation(vec);
    }
    
    void ClientNode::set_scale(Vec3D vec) {
        Object::set_scale(vec);
    }

	void ClientNode::create_object(const Tete&) {
	}

	void ClientNode::update_object(const Tete&) {
	}

	void ClientNode::update_object_matrix(const Tete&) {
	}

	void ClientNode::parse_create_tete(const Tete& tete) {
		if (tete.attr()["type"].empty())
			return;

		std::string type = tete.attr()["type"].asString();
		ID id = tete.id();
		// Listen for UserNode
		// Create CameraNode
		if (type == "UserNode") {
			UserNodePtr node(new ClientUserNode(tete.id()));

            Client::user_identity_->set_object(node);

            Client::renderer() += node;

            ObjectController::instance() += node;
            Identity::create_identity(id, node);

            node->create_object(tete);

			CameraNode::request_create(node);
		} else if (type == "CameraNode") {
			CameraNodePtr cam_node(new CameraNode(tete.id()));

			Client::renderer().set_camera(cam_node);
			Client::set_active_camera(cam_node.get());

            ObjectController::instance() += cam_node;
            Identity::create_identity(tete.id(), cam_node);

            cam_node->create_object(tete);

			// Everything has been created, we don't need to listen for any more tetes
			initializing_ = false;
		}
	}

	bool ClientNode::is_our_tete(const Tete& tete) {
		if (tete.attr()["links"].empty())
			return false;

		ID owner_id = tete.attr()["links"][0u]["id"].asString();

		return owner_id == Client::user_identity().id();
	}

	void ClientNode::parse_list_rooms(const Tete& tete) {
		if (tete.value()["rooms"].empty()) {
			Grids::Interface::instance().request_create_room();
			return;
		}

		if (Client::user_identity().current_room().empty()) {
			Client::user_identity_->set_current_room(
				tete.value()["rooms"][0u].asString());
			
			UserNode::request_create(Client::user_identity().id());
		}
	}

	void ClientNode::parse_create_room(const Tete& tete) {
		//if (tete.value()["success"].empty())
		//	return;
		Grids::Interface::instance().request_list_rooms();
	}

    void ClientNode::activate(const Identity& ident) {
        GridsNetworkItem::activate(ident);
    }
}