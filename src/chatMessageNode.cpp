
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>
#include <cinder/Color.h>

#include <chatMessageNode.h>
#include <tete.h>
#include <client.h>
#include <link.h>
#include <utility.h>

namespace Atelier {
    ChatMessageNode::ChatMessageNode(const ID& in_id) : Node(in_id) {
        //ci::app::console() << "Creating ChatMessageNode" << std::endl;
        text_size_ = 48.0f;
        text_ = "";

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

    Rect ChatMessageNode::bounding_rect() const {
        if (text_texture_.get() == NULL)
            return Rect();

        float text_width = static_cast<float>(text_texture_->getWidth());
        float text_height = static_cast<float>(text_texture_->getHeight());
        float pos_x = position().x;
        float pos_y = position().y;

        return Rect(pos_x - text_width / 2.0f,
            pos_y - text_height / 2.0f,
            pos_x + text_width / 2.0f,
            pos_y + text_height / 2.0f);
    }

    Prism ChatMessageNode::bounding_prism() const {
        return Prism();
    }

    AABox ChatMessageNode::bounding_aabox() const {
        return AABox();
    }

    void ChatMessageNode::create_object(const Tete& tete) {
        Object::create_object(tete);

        text_ = tete.attr()["text"].asString();

        generate_layout();
        generate_texture();
    }

    void ChatMessageNode::update_object(const Tete& tete) {
        Object::update_object(tete);

        if (!tete.attr()["text"].empty()) {
            text_ = tete.attr()["text"].asString();
            generate_texture(); // update texture
        }
    }

    void ChatMessageNode::update_object_matrix(const Tete& tete) {

    }

    void ChatMessageNode::receive_tete(const Tete& tete) {

    }

    void ChatMessageNode::render(RenderDimension dim, RenderPass) {
        if (text_texture_ == NULL)
            return;
        
        // For now limit drawing to 2 dimensions
        if (dim == TWO) {
            init_matrix();
            draw_text();
            restore_matrix();
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
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		ci::gl::enableDepthWrite(false);
        text_texture_->enableAndBind();
        draw_text_billboard();
        text_texture_->disable();
    }

    void ChatMessageNode::draw_text_billboard() {
        float w = static_cast<float>(text_texture_->getWidth());
        float h = static_cast<float>(text_texture_->getHeight());

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
        glVertex2f(0.0f, h);

        glTexCoord2f( 1, 1 );
        glVertex2f(w, h);

        glTexCoord2f( 1, 0 );
        glVertex2f(w, 0.0f);

        glTexCoord2f( 0, 0 );
        glVertex2f(0.0f, 0.0f);
        glEnd();
        //glDisable( GL_TEXTURE_2D ); // called in draw_text();
    }

    void ChatMessageNode::generate_layout() {
        layout_ = LayoutPtr(new ci::TextLayout());
        layout_->setFont(ci::Font("HelveticaNeue", text_size_));
        layout_->setColor(ci::Color(1, 1, 1));
    }

    void ChatMessageNode::generate_texture() {
        generate_layout();

        layout_->clear(ci::ColorA());
        layout_->addCenteredLine(text_);
        
        text_texture_ = TexturePtr(new ci::gl::Texture(layout_->render(true)));
        text_texture_->disable(); // Required for bug. TODO: remove
    }

    
}