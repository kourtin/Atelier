
#include <clientNode.h>
#include <tete.h>
#include <client.h>
#include <userNode.h>

namespace Atelier {
	ClientNode::ClientNode(const ID& new_id) : Object(new_id) {

	}

	ClientNode::~ClientNode() {
	}

	void ClientNode::receive_tete(const Tete& tete) {
		if (!is_our_tete(tete))
			return;

		if (tete.type() == Tete::CREATE)
			parse_create_tete(tete);
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
			if (!tete.has_links())
				return;

			if (tete.links()[0].actor() != Client::user_identity())
				return;

			UserNode* node = new UserNode(tete.id());
			node->create_object(tete);

			CameraNode::request_create(node);
			
		} else if (type == "CameraNode") {

		}
	}

	bool ClientNode::is_our_tete(const Tete& tete) {
		if (tete.attr()["links"].empty())
			return false;

		ID owner_id = tete.attr()["links"][0u]["id"].asString();

		return owner_id == Client::user_identity().id();
	}



}