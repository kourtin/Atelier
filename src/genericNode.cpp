
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/CinderResources.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Camera.h"

#include <genericNode.h>
#include <cinderGraphicItem.h>
#include <basicApp.h>
#include <renderer.h>
#include <cameraNode.h>
#include <client.h>
#include <identity.h>
#include <aaBox.h>
#include <gridsNetworkItem.h>
#include <tete.h>
#include <grids/interface.h>
#include <node.h>
#include <link.h>

namespace Atelier {
    GenericNode::GenericNode(const ID& new_id) : Node(new_id) {
        set_position(Vec3D(0.0, 0.0, 0.0));
        set_rotation(Vec3D(0.0, 0.0, 0.0));
        set_scale(Vec3D(10.0, 10.0, 10.0));
        text_size_ = 48.0f;
        text_string_ = "?";
        billboard_ = false;
        detect_selection_ = true;
    }

    GenericNode::~GenericNode() {
    }

    void GenericNode::init() {
        set_detect_selection(true);

        layout_.setFont(ci::Font("HelveticaNeue", text_size_));
        layout_.setColor(ci::Color(1, 1, 1));
        
        layout_.addCenteredLine(text_string_);

	    text_ = ci::gl::Texture(layout_.render(true));
        //text_ = ci::gl::Texture(ci::loadImage(app_.loadResource(
        //    CINDER_RESOURCE( ../resources/, earthNormal.png, 131, PNG ))));
    }

    Prism GenericNode::bounding_prism() const {
        return CinderGraphicItem::bounding_prism();
    }

    Rect GenericNode::bounding_rect() const {
        return CinderGraphicItem::bounding_rect();
    }

    AABox GenericNode::bounding_aabox() const {
        return CinderGraphicItem::bounding_aabox();
    }

    void GenericNode::set_billboard(bool bill) {
        billboard_ = bill;
    }

    void GenericNode::receive_tete(const Tete& tete) {
    }

    void GenericNode::render(RenderDimension dim, RenderPass pass) {
        init_matrix();
        draw_box();
        draw_text();
        restore_matrix();
        draw_aabox();
    }

    void GenericNode::init_matrix() {
        ci::gl::pushModelView();
        ci::gl::translate(position());
        ci::gl::scale(scale());
    }

    void GenericNode::restore_matrix() {
        ci::gl::popModelView();
    }

    void GenericNode::draw_aabox() {
        glColor4f( 1.f, 0.4f, 0.4f, 1.0f );
        glLineWidth(1.0f);
        bounding_aabox().glDraw();
    }

    void GenericNode::draw_box() {
        static GLfloat vertices[] = {	1, 1, 1,  1,-1, 1,  1,-1,-1,  1, 1,-1,
						        -1, 1, 1, -1,-1, 1, -1,-1,-1, -1, 1,-1 };

        static GLubyte elements[] = {	0, 1, 1, 2, 2, 3, 3, 0,
						        4, 5, 5, 6, 6, 7, 7, 4,
						        0, 4, 1, 5, 2, 6, 3, 7 };

        glColor4f( 1.f, 0.4f, 0.4f, 1.0f );
        glLineWidth(1.0f);

#if ! defined( CINDER_GLES )
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 10, 0xAAAA );
#endif

        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 3, GL_FLOAT, 0, vertices );
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, elements);
        glDisableClientState( GL_VERTEX_ARRAY );

#if ! defined( CINDER_GLES )
        glDisable( GL_LINE_STIPPLE );
#endif
    }

    void GenericNode::draw_text() {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		ci::gl::enableDepthWrite( false );
        glEnable(GL_TEXTURE_2D);
        text_.bind();

        billboard_ ? draw_text_billboard() : draw_text_static();
    }

    void GenericNode::draw_text_billboard() {
        float w = text_.getWidth();
        float h = text_.getHeight();

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
        glDisable( GL_TEXTURE_2D );
    }

    void GenericNode::draw_text_static() {
        float w = text_.getWidth();
        float h = text_.getHeight();

        glBegin( GL_QUADS );
        glTexCoord2f( 0, 1 );
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glTexCoord2f( 1, 1 );
        glVertex3f(1.0f, 1.0f, 0.0f);
        glTexCoord2f( 1, 0 );
        glVertex3f(1.0f, -1.0f, 0.0f);
        glTexCoord2f( 0, 0 );
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glEnd();
        glDisable( GL_TEXTURE_2D );
    }

    void GenericNode::activate(const Identity& ident) {
        billboard_ = !billboard_;
        GridsNetworkItem::activate(ident);
    }

    void GenericNode::create_object(const Tete& tete) {
        Object::create_object(tete);

		if (tete.has_matrix()) {
			set_position(tete.position());
			set_rotation(tete.rotation());
			set_scale(tete.scale());
		}
    }

    void GenericNode::update_object(const Tete&) {
    }

    void GenericNode::update_object_matrix(const Tete&) {
    }

    void GenericNode::request_create_object(const Value&) {
    }

    void GenericNode::request_update_object(const Value&) {
    }

	void GenericNode::request_create_object(Vec3D pos) {
		Tete request;
		request.set_position(pos);
		request.links().push_back(new Link(&(Client::user_identity()),
			LinkFlags(true, true, true)));
		//request.links().push_back(&(Client::user_identity())); // Presumable the user created this
		request.attr()["type"] = "GenericNode";

		GridsNetworkItem::request_create_object(request);
		// Maybe send the tete elsewhere
	}
}