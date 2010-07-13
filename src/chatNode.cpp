
#include <cinder/app/KeyEvent.h>

#include <chatNode.h>
#include <client.h>
#include <link.h>

namespace Atelier {
    ChatNode::ChatNode(const ID& in_id) : Object(in_id) {
        ci::app::console() << "Creating ChatNode" << std::endl;
        active_node_ = NULL;
    }

    ChatNode::~ChatNode() {
    }

    void ChatNode::request_create() {
        Tete request;

		request.links().push_back(new Link(&(Client::user_identity()),
			LinkFlags(true, true, true)));
		request.attr()["type"] = "ChatNode";

		GridsNetworkItem::request_create_object(request);
    }

    void ChatNode::create_object(const Tete& tete) {
        Object::create_object(tete);

        const Object* obj = Client::user_identity().object();

        if (obj == NULL) {
            ci::app::console() << "CRITICAL: unable to get UserNode for client" <<
                std::endl;
            return;
        }
        
        const InteractItem* item = dynamic_cast<const InteractItem*>(obj);

        if (item == NULL) {
            ci::app::console() << "CRITICAL: unable to get UserNode for client" <<
                std::endl;
            return;
        }

        container_ = item;
    }

    void ChatNode::update_object(const Tete& tete) {
    }

    void ChatNode::update_object_matrix(const Tete& tete) {
    }

    void ChatNode::receive_tete(const Tete& tete) {
    }

    void ChatNode::activate(const Identity& ident) {
    }

    bool ChatNode::keyDown(ci::app::KeyEvent event) {
        if (event.getCode() == ci::app::KeyEvent::KEY_RETURN) {
            if (active_node_ != NULL) {
                // Finish updating and deactivate
                active_node_ = NULL;
                text_buffer_.clear();
            }
        } else {
            if (active_node_ == NULL) {
                // Request create a node, but make sure to update it    
            } else {
                // Update the current node, and send an update request
            }
        }

        text_buffer_ += event.getChar();

        return false;
    }

    Rect ChatNode::bounding_rect() const {
        return Rect();
    }

    const InteractItem& ChatNode::container() const {
        return *container_;
    }
}