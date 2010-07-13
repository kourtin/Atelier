
#include <cinder/app/KeyEvent.h>

#include <chatNode.h>
#include <client.h>
#include <link.h>
#include <teteManager.h>
#include <utility.h>
#include <chatMessageNode.h>

namespace Atelier {
    ChatNode::ChatNode(const ID& in_id) : Object(in_id) {
        ci::app::console() << "Creating ChatNode" << std::endl;
        active_node_ = NULL;
        container_ = NULL;
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
        // Look out for the creation of a new object
        if (tete.type() == Tete::CREATE && tete.id() == active_node_create_id_) {
            // Great, now I know it's arrived, when do I grab the object?
            TeteManager::instance() -= this; // don't need to listen anymore
            active_node_create_id_.clear();
        }
    }

    void ChatNode::activate(const Identity& ident) {
    }

    bool ChatNode::keyDown(ci::app::KeyEvent event) {
        if (event.getCode() == ci::app::KeyEvent::KEY_RETURN) {
            if (active_node_ != NULL) {
                // Finish updating and deactivate
                active_node_ = NULL;
                text_buffer_.clear();
                active_node_create_id_.clear();
            }
        } else {
            text_buffer_ += event.getChar();

            if (active_node_create_id_.empty()) {
                // Request create a node, but make sure to update it
                TeteManager::instance() += this;
                active_node_create_id_ = Utility::create_uuid();
                ChatMessageNode::request_create(active_node_create_id_,
                    *identity(), text_buffer_, std::deque<Link*>());
            } else if (active_node_ != NULL) {
                // Update the current node, and send an update request
                ChatMessageNode::request_update_text(*(active_node_->identity()),
                    text_buffer_);
            }
        }

        return false;
    }

    Rect ChatNode::bounding_rect() const {
        return Rect();
    }

    const InteractItem& ChatNode::container() const {
        return *container_;
    }
}