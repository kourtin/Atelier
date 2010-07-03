
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

namespace Atelier {
	ClientNode::ClientNode(const ID& new_id) : Object(new_id) {
		initializing_ = true;
	}

	ClientNode::~ClientNode() {
	}

	void ClientNode::receive_tete(const Tete& tete) {
		ci::app::console() << "ClientNode receiving tete:" << std::endl <<
			tete.value().toStyledString() << std::endl;
		
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

	void ClientNode::create_object(const Tete& tete) {
	}

	void ClientNode::update_object(const Tete& tete) {
	}

	void ClientNode::update_object_matrix(const Tete& tete) {
	}

	void ClientNode::parse_create_tete(const Tete& tete) {
		if (tete.attr()["type"].empty())
			return;

		std::string type = tete.attr()["type"].asString();
		ID id = tete.id();
		// Listen for UserNode
		// Create CameraNode
		if (type == "UserNode") {
			UserNode* node = new ClientUserNode(tete.id());
			node->create_object(tete);

            Client::renderer() += node;

			CameraNode::request_create(node);
		} else if (type == "CameraNode") {
			CameraNode* cam_node = new CameraNode(tete.id());
			cam_node->create_object(tete);

			Client::renderer().set_camera(cam_node);
			Client::set_active_camera(cam_node);
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


}