
#include <userNode.h>
#include <tete.h>
#include <link.h>
#include <client.h>

namespace Atelier {
	UserNode::UserNode(const ID& in_id) : Node(in_id) {
		ci::app::console() << "Creating UserNode" << std::endl;
	}

	UserNode::~UserNode() {
	}

	void UserNode::request_create(const ID& req_id) {
		Tete request;
		
		request.links().push_back(new Link(&(Client::user_identity()),
			LinkFlags(true, true, true)));
		request.attr()["type"] = "UserNode";

		GridsNetworkItem::request_create_object(request);
	}

	Prism UserNode::bounding_prism() const {
		return Prism();
	}

	AABox UserNode::bounding_aabox() const {
		return AABox();
	}

	Rect UserNode::bounding_rect() const {
		return Rect();
	}

	void UserNode::render(RenderDimension, RenderPass) {
	}

	void UserNode::create_object(const Tete&) {
	}

	void UserNode::update_object(const Tete&) {
	}

	void UserNode::update_object_matrix(const Tete&) {
	}

	void UserNode::receive_tete(const Tete&) {
	}
}