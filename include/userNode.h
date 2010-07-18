
// This is sort of like your buddy icon, or something

#pragma once

#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>

#include <node.h>

namespace Atelier {
	class UserNode : public Node {
	public:
		UserNode(const ID&);
		virtual ~UserNode();

		std::string type() const { return "UserNode"; }

		static void request_create(const ID&);

		virtual void create_object(const Tete&);
		virtual void update_object(const Tete&);
		virtual void update_object_matrix(const Tete&);

		virtual void receive_tete(const Tete&);

        virtual Prism bounding_prism() const;
        virtual AABox bounding_aabox() const;
		virtual Rect bounding_rect() const;

		virtual void render(RenderDimension, RenderPass);

    private:
        void draw_box();
        void draw_text();
        void draw_text_billboard();
        void init_matrix();
        void restore_matrix();

        ci::gl::Texture* user_name_texture_;
        ci::TextLayout* layout_;
        float text_size_;

        std::string user_name_;
	};

    typedef std::tr1::shared_ptr<UserNode> UserNodePtr;
}