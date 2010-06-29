
#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

#include "define.h"
#include "node.h"
#include "prism.h"
#include "aaBox.h"

class CinderApp;

namespace Atelier {
    class Tete;
    class Identity;

    class GenericNode : public Node {
    public:
        GenericNode(const ID&);
        virtual ~GenericNode();
        void init();

        virtual Rect bounding_rect() const;
        virtual Prism bounding_prism() const;
        virtual AABox bounding_aabox() const;

        virtual void receive_tete(const Tete&);
        virtual void activate(const Identity&);

        virtual void render(RenderDimension, RenderPass);

        void set_billboard(bool);

		void create_object(const Tete&);

		static void request_create_object(Vec3D pos);

		virtual std::string type() { return "GenericNode"; }

        virtual void request_create_object(const Value&);
        virtual void request_update_object(const Value&);
        virtual void update_object(const Tete&);
        virtual void update_object_matrix(const Tete&);

    private:
        void draw_box();
        void draw_text();
        void draw_text_static();
        void draw_text_billboard();
        void draw_aabox();
        void init_matrix();
        void restore_matrix();

        float text_size_;
        ci::gl::Texture text_;
        ci::TextLayout layout_;
        std::string text_string_;
        bool billboard_;
    };
}