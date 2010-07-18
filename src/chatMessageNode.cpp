
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>
#include <cinder/Color.h>

#include <chatMessageNode.h>
#include <tete.h>
#include <client.h>
#include <link.h>
#include <utility.h>
#include <chatOrganizer.h>
#include <chatNode.h>

namespace Atelier {
    ChatMessageNode::ChatMessageNode(const ID& in_id) : Node2D(in_id) {
        detect_selection_ = true;
        draggable_ = true;
        text_size_ = 22.0f;
        text_ = "";
        velocity_ = Vec3D(0.0f, 0.0f, 0.0f);

        set_position(Vec3D(100.f, 100.f, 0.0f));
    }

    ChatMessageNode::~ChatMessageNode() {
    }

    void ChatMessageNode::request_create(const ID& req_id, const Identity& chat_ident,
        std::string initial_text, const std::deque<LinkPtr>& links) {
        Tete request;
        request.value()["id"] = req_id;
        // request.set_position(organizer_->get_next_position());
        for(std::deque<LinkPtr>::const_iterator it = links.begin(); it != links.end(); ++it) {
            request.links().push_back(*it);
        }
        LinkPtr chat_link(new Link(&chat_ident, LinkFlags(true, true)));
        ci::app::console() << "Requesting create chat mess node" << std::endl;
        ci::app::console() << "ID = " << chat_ident.id() << std::endl;

        request.links().push_back(chat_link);
        request.attr()["type"] = "ChatMessageNode";
        request.attr()["text"] = initial_text;

        GridsNetworkItem::request_create_object(request);
    }

    // Each update event has a unique update key
    void ChatMessageNode::request_update_text(const Identity& node_ident, 
        const std::string& new_text) {
        Tete request;

        request.value()["id"] = node_ident.id();
        request.attr()["update_key"] = Utility::create_uuid();
        request.attr()["type"] = "ChatMessageNode";
        request.attr()["text"] = new_text;

        GridsNetworkItem::request_update_object(request);
    }

    void ChatMessageNode::request_finish_update(const Identity& node_ident,
        const std::string& final_text) {
        Tete request;

        request.value()["id"] = node_ident.id();
        request.attr()["update_key"] = Utility::create_uuid();
        request.attr()["type"] = "ChatMessageNode";
        request.attr()["text"] = final_text;
        request.attr()["finished"] = true;

        GridsNetworkItem::request_update_object(request);
    }

    // Bounding rect is not translated to position
    Rect ChatMessageNode::bounding_rect() const {
        ScopedLock l(text_texture_mutex_);

        if (text_texture_.get() == NULL)
            return Rect();

        float text_width = static_cast<float>(text_texture_->getWidth());
        float text_height = static_cast<float>(text_texture_->getHeight());

        return Rect(-text_width / 2.0f, -text_height / 2.0f,
            text_width / 2.0f, text_height / 2.0f);
    }

    Prism ChatMessageNode::bounding_prism() const {
        return Prism();
    }

    AABox ChatMessageNode::bounding_aabox() const {
        return AABox();
    }

    void ChatMessageNode::create_object(const Tete& tete) {
        Node2D::create_object(tete);

        // Register this object with the chat
        register_with_chat_node(tete);

        text_ = tete.attr()["text"].asString();

        generate_layout();
        generate_texture();
    }

    void ChatMessageNode::register_with_chat_node(const Tete& tete) {
        std::tr1::shared_ptr<ChatNode> chat_node;
        ci::app::console() << "Registering chat node" << std::endl;
        // Find the chat this object is a part of
        for (LinkList::const_iterator it = tete.links().begin(); it != tete.links().end();
            ++it) {
            ci::app::console() << (*it)->actor().id() << std::endl;

            ObjectPtr obj = ObjectController::instance().get_object_from_id((*it)->actor().id());

            //std::tr1::shared_ptr<ChatNode> chat_node = std::dynamic_pointer_cast<ChatNode>(obj);

            //ci::app::console() << obj->type() << std::endl;

            //if (chat_node.get() != NULL)
            if (obj->type() == "ChatNode") {
                chat_node = std::dynamic_pointer_cast<ChatNode>(obj);
                break;
            }
        }

        if (chat_node.get() == NULL) {
            ci::app::console() << "[WRN] CRITICAL: message received with no ChatNode" << std::endl;
            return;
        }

        ObjectPtr this_ptr = ObjectController::instance().get_object_from_id(id());
        ChatMessageNodePtr msg_ptr = std::dynamic_pointer_cast<ChatMessageNode>(this_ptr);
        chat_node->add_chat_message_node(msg_ptr);
    }

    void ChatMessageNode::update_object(const Tete& tete) {
        Node2D::update_object(tete);

        if (!tete.attr()["text"].empty()) {
            text_ = tete.attr()["text"].asString();
            generate_texture(); // update texture
        }
    }

    void ChatMessageNode::update_object_matrix(const Tete& tete) {

    }

    void ChatMessageNode::receive_tete(const Tete& tete) {

    }

    Vec3D ChatMessageNode::velocity() const {
        ScopedLock l(velocity_mutex_);

        return velocity_;
    }

    void ChatMessageNode::set_velocity(Vec3D vel) {
        ScopedLock l(velocity_mutex_);

        velocity_ = vel;
    }

    void ChatMessageNode::render(RenderDimension dim, RenderPass) {
        ScopedLock l(text_texture_mutex_);

        if (text_texture_ == NULL)
            return;
        
        // For now limit drawing to 2 dimensions
        if (dim == TWO) {
            init_matrix();
            draw_text();
            restore_matrix();

            draw_links();
        }
	}

    void ChatMessageNode::init_matrix() {
        ci::gl::pushModelView();
        ci::gl::translate(position());
        ci::gl::scale(scale());
    }

    void ChatMessageNode::restore_matrix() {
        ci::gl::popModelView();
    }

    void ChatMessageNode::draw_text() {
        ScopedLock l(text_texture_mutex_);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		ci::gl::enableDepthWrite(false);
        text_texture_->enableAndBind();
        draw_text_billboard();
        text_texture_->disable();
    }

    void ChatMessageNode::draw_text_billboard() {
        ScopedLock l(text_texture_mutex_);

        float w = static_cast<float>(text_texture_->getWidth());
        float h = static_cast<float>(text_texture_->getHeight());
        float w_2 = w / 2.0f;
        float h_2 = h / 2.0f;

        Vec3D right;
        Vec3D up;
        const Vec3D& sRight = Client::renderer().billboard_right();
        const Vec3D& sUp = Client::renderer().billboard_up();

        float perLeft = -1.0f;
        float perRight = 1.0f;
        right = sRight;
        up = sUp * h / w;

        glBegin( GL_QUADS );
        glTexCoord2f( 0, 1 );
        glVertex2f(-w_2, h_2);

        glTexCoord2f( 1, 1 );
        glVertex2f(w_2, h_2);

        glTexCoord2f( 1, 0 );
        glVertex2f(w_2, -h_2);

        glTexCoord2f( 0, 0 );
        glVertex2f(-w_2, -h_2);
        glEnd();
        //glDisable( GL_TEXTURE_2D ); // called in draw_text();
    }

    void ChatMessageNode::draw_links() {
        glLineWidth(0.1);
        glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
        glBegin(GL_LINES);
        Vec3D my_pos = position();
        Vec3D pos;
        for (std::list<LinkConstPtr>::const_iterator it = links().begin();
            it != links().end(); ++it) {
            if ((*it)->actor().object()->type() != type())
                continue;

            pos = (*it)->actor().object()->position();
            glVertex2f(my_pos.x, my_pos.y);
            glVertex2f(pos.x, pos.y);
        }
        glEnd();
    }

    void ChatMessageNode::generate_layout() {
        layout_ = LayoutPtr(new ci::TextLayout());
        layout_->setFont(ci::Font("HelveticaNeue", text_size_));
        layout_->setColor(ci::Color(1, 1, 1));
    }

    void ChatMessageNode::generate_texture() {
        generate_layout();

        ScopedLock l(text_texture_mutex_);

        layout_->clear(ci::ColorA());
        layout_->addCenteredLine(text_);
        
        text_texture_ = TexturePtr(new ci::gl::Texture(layout_->render(true)));
        text_texture_->disable(); // Required for bug. TODO: remove
    }

    const InteractItem& ChatMessageNode::container() const {
        return *(container_.get());
    }

    void ChatMessageNode::update_velocity() {
        //ci::app::console() << position().x << " : " << position().y <<
        //    " : " << position().z << std::endl;
        //velocity_.z = 0.0f;
        lock_position();
        if (ChatOrganizer::within_screen_bounds(position(), 
            scale(), bounding_rect()))
            set_position(position() + velocity_);
        unlock_position();
    }

    void ChatMessageNode::activate(const Identity& ident) {
        //lock_position();
        //set_position(position() + Vec3D(5.0f, 5.0f, 0.0f));
        //unlock_position();
    }

    void ChatMessageNode::drag_start(ci::app::MouseEvent mouse_event) {
        set_position(Vec3D(static_cast<float>(mouse_event.getPos().x), 
            static_cast<float>(mouse_event.getPos().y), 0.0f));
    }

    void ChatMessageNode::dragging(ci::app::MouseEvent mouse_event) {
        set_position(Vec3D(static_cast<float>(mouse_event.getPos().x), 
            static_cast<float>(mouse_event.getPos().y), 0.0f));
    }
}