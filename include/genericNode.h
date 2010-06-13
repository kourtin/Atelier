
#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

#include "define.h"
#include "node.h"
#include "prism.h"

class CinderApp;

namespace Atelier {
    class GenericNode : public Node {
    public:
        GenericNode(ID);
        virtual ~GenericNode();
        void init();

        virtual Rect bounding_rect();
        virtual Prism bounding_prism();

        virtual void receive_tete(const Tete&);

        virtual void render(RenderDimension, RenderPass);

        void set_billboard(bool);

    private:
        void draw_box();
        void draw_text();
        void draw_text_static();
        void draw_text_billboard();
        void init_matrix();
        void restore_matrix();

        float text_size_;
        ci::gl::Texture text_;
        ci::TextLayout layout_;
        std::string text_string_;
        bool billboard_;
    };
}