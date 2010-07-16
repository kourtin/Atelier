
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
        active_node_identity_ = NULL;
        unsent_chars_ = false;
    }

    ChatNode::~ChatNode() {
    }

    const ID& ChatNode::id() {
        return Object::id();
    }

    void ChatNode::request_create() {
        Tete request;

        LinkPtr client_link(new Link(&(Client::user_identity()),
			LinkFlags(true, true, true)));
		request.links().push_back(client_link);
		request.attr()["type"] = "ChatNode";

		GridsNetworkItem::request_create_object(request);
    }

    void ChatNode::create_object(const Tete& tete) {
        Object::create_object(tete);

        const ObjectPtr obj = Client::user_identity().object();

        if (obj.get() == NULL) {
            ci::app::console() << "CRITICAL: unable to get UserNode for client" <<
                std::endl;
            return;
        }
        
        const std::tr1::shared_ptr<InteractItem> item = 
            std::dynamic_pointer_cast<InteractItem>(obj);

        if (item.get() == NULL) {
            ci::app::console() << "CRITICAL: unable to get UserNode for client" <<
                std::endl;
            return;
        }

        container_ = item;
    }

    std::list<const Link*>& ChatNode::links() {
        return Object::links();
    }

	const std::list<const Link*>& ChatNode::links() const {
        return Object::links();
    }

    Vec3D ChatNode::position() const {
        return Object::position();
    }

    Vec3D ChatNode::rotation() const {
        return Object::rotation();
    }

    Vec3D ChatNode::scale() const {
        return Object::scale();
    }

    void ChatNode::set_position(Vec3D vec) {
        Object::set_position(vec);
    }

    void ChatNode::set_rotation(Vec3D vec) {
        Object::set_rotation(vec);
    }
    
    void ChatNode::set_scale(Vec3D vec) {
        Object::set_scale(vec);
    }

    void ChatNode::update_object(const Tete& tete) {
    }

    void ChatNode::update_object_matrix(const Tete& tete) {
    }

    void ChatNode::activate(const Identity& ident) {
    }

    // This might not be neaded
    void ChatNode::receive_tete(const Tete& tete) {
        // Look out for the creation of a new object
        if (tete.type() == Tete::CREATE && active_node_identity_ != NULL && 
            tete.id() == active_node_identity_->id()) {
            // Great, now I know it's arrived, when do I grab the object?
                TeteManager::instance() -= ObjectController::instance()[id()]; // don't need to listen anymore

            if (unsent_chars_ && active_node_identity_->object() != NULL) {
                // The identity may not have been created yet, ie only works some of the 
                // time. Needs some kind of callback mechanism...
                ChatMessageNode::request_update_text(*active_node_identity_, text_buffer_);
            }
        }
    }

    // TODO: Fix this memory leak from new Link(...)
    bool ChatNode::keyDown(ci::app::KeyEvent event) {
        if (event.getCode() == ci::app::KeyEvent::KEY_RETURN) {
            if (active_node_identity_ != NULL && 
                active_node_identity_->object() != NULL) {
                // Finish updating by sending finished flag and deactivate
                ChatMessageNode::request_finish_update(*active_node_identity_, 
                    text_buffer_);

                active_node_ = NULL;
                text_buffer_.clear();
                active_node_identity_ = NULL;
            }
        } else {
            text_buffer_ += event.getChar();
            unsent_chars_ = true;

            if (active_node_identity_ == NULL) {
                // Request create a node, but make sure to update it
                TeteManager::instance() += ObjectController::instance()[id()];
                ID create_id_ = Utility::create_uuid();
                active_node_identity_ = Identity::create_identity(create_id_, NULL);

                LinkPtr user_link(new Link(&(Client::user_identity()),
                    LinkFlags(true, true, true)));
                std::deque<LinkPtr> temp_links;
                temp_links.push_back(user_link);

                ChatMessageNode::request_create(create_id_,
                    *identity(), text_buffer_, temp_links);

            } else if (active_node_identity_ != NULL && 
                active_node_identity_->object() != NULL) {
                // Update the current node, and send an update request
                ChatMessageNode::request_update_text(*active_node_identity_,
                    text_buffer_);
                unsent_chars_ = false;
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