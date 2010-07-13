
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>
#include <cinder/Color.h>

#include <chatMessageNode.h>
#include <tete.h>
#include <client.h>
#include <link.h>

namespace Atelier {
    ChatMessageNode::ChatMessageNode(const ID& in_id) : Node(in_id) {
        //ci::app::console() << "Creating ChatMessageNode" << std::endl;
        text_size_ = 48.0f;
        text_texture_ = NULL;
        layout_ = NULL;
        text_ = "";
    }

    ChatMessageNode::~ChatMessageNode() {
    }

    void ChatMessageNode::request_create(const ID& req_id, const Identity& chat_ident,
        std::string initial_text, const std::deque<Link*>& links) {
        Tete request;
        request.value()["id"] = req_id;
        // request.set_position(organizer_->get_next_position());
        for(std::deque<Link*>::const_iterator it = links.begin(); it != links.end(); ++it) {
            request.links().push_back(*it);
        }
        request.links().push_back(new Link(&chat_ident, LinkFlags(true, true)));
        request.attr()["type"] = "ChatMessageNode";
        request.attr()["text"] = initial_text;

        GridsNetworkItem::request_create_object(request);
    }

    void ChatMessageNode::request_update_text(const Identity& node_ident, 
        const std::string& new_text_) {
        Tete request;

        request.value()["id"] = node_ident.id();
        request.attr()["type"] = "ChatMessageNode";
        request.attr()["text"] = new_text_;

        GridsNetworkItem::request_update_object(request);
    }

    Rect ChatMessageNode::bounding_rect() const {
        return Rect();
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
            // update texture
        }
    }

    void ChatMessageNode::update_object_matrix(const Tete& tete) {

    }

    void ChatMessageNode::receive_tete(const Tete& tete) {

    }

    void ChatMessageNode::render(RenderDimension dim, RenderPass) {
        // For now limit drawing to 2 dimensions
        if (dim == RenderDimension::TWO_D) {
            init_matrix();
            draw_text();
            restore_matrix();
        } else if (dim == RenderDimension::THREE_D) {

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
        float w = text_texture_->getWidth();
        float h = text_texture_->getHeight();

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
        glVertex3f(right.x * perLeft + up.x * perLeft, 
            right.y * perLeft + up.y * perLeft, 
            right.z * perLeft + up.z * perLeft);
        glTexCoord2f( 1, 1 );
        glVertex3f(right.x * perRight + up.x * perLeft, 
            right.y * perRight + up.y * perLeft, 
            right.z * perRight + up.z * perLeft);
        glTexCoord2f( 1, 0 );
        glVertex3f(right.x * perRight + up.x * perRight, 
            right.y * perRight + up.y * perRight, 
            right.z * perRight + up.z * perRight);
        glTexCoord2f( 0, 0 );
        glVertex3f(right.x * perLeft + up.x * perRight,	
            right.y * perLeft + up.y * perRight, 
            right.z * perLeft + up.z * perRight);
        glEnd();
        //glDisable( GL_TEXTURE_2D ); // called in draw_text();
    }

    void ChatMessageNode::generate_layout() {
        layout_ = new ci::TextLayout();
        layout_->setFont(ci::Font("HelveticaNeue", text_size_));
        layout_->setColor(ci::Color(1, 1, 1));
    }

    void ChatMessageNode::generate_texture() {
        if (layout_ == NULL)
            generate_layout();

        layout_->clear(ci::ColorA(0.0f, 0.0f, 0.0f, 1.0f));
        layout_->addCenteredLine(text_);

        if (text_texture_ != NULL)
            delete text_texture_;

        text_texture_ = new ci::gl::Texture(layout_->render(true));
    }

    
}