
#include <userNode.h>
#include <tete.h>
#include <link.h>
#include <client.h>

namespace Atelier {
	UserNode::UserNode(const ID& in_id) : Node(in_id) {
		text_size_ = 48.0f;
	}

	UserNode::~UserNode() {
	}

	void UserNode::request_create(const ID& req_id) {
		Tete request;
	
        request.value()["id"] = req_id;
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

	void UserNode::create_object(const Tete& tete) {
        const Identity* creator = Tete::get_creator(tete);

        if (creator == NULL) {
            ci::app::console() << "CRITICAL: unable to find identity for UserNode"
                << std::endl;
            return;
        }

        user_name_ = creator->name();

        layout_.setFont(ci::Font("HelveticaNeue", text_size_));
        layout_.setColor(ci::Color(1, 1, 1));
        
        layout_.addCenteredLine(user_name_);

        // TODO: understand why this doesn't work
	    //user_name_texture_ = ci::gl::Texture(layout_.render(true));
	}

	void UserNode::update_object(const Tete&) {
	}

	void UserNode::update_object_matrix(const Tete&) {
	}

	void UserNode::receive_tete(const Tete&) {
	}

	void UserNode::render(RenderDimension, RenderPass) {
        init_matrix();
        draw_box();
        draw_text();
        restore_matrix();
	}

    void UserNode::init_matrix() {
        ci::gl::pushModelView();
        ci::gl::translate(position());
        ci::gl::scale(scale());
    }

    void UserNode::restore_matrix() {
        ci::gl::popModelView();
    }

    void UserNode::draw_box() {
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

    void UserNode::draw_text() {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		ci::gl::enableDepthWrite( false );
        glEnable(GL_TEXTURE_2D);
        user_name_texture_.bind();

        draw_text_billboard();
    }

    void UserNode::draw_text_billboard() {
        float w = user_name_texture_.getWidth();
        float h = user_name_texture_.getHeight();

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
}