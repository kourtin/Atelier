
#pragma once



#include <cinder/Text.h>
#include <cinder/gl/Texture.h>

#include <node.h>

namespace Atelier {
    class Link;

    class ChatMessageNode : public Node {
    public:
        ChatMessageNode(const ID&);
        virtual ~ChatMessageNode();

        static void request_create(const ID& req_id, const Identity& chat_ident, 
            std::string initial_text, const std::deque<Link*>& links);
        static void request_update_text(const Identity& node_ident, 
            const std::string&);

        std::string type() { return "ChatMessageNode"; }

        Rect bounding_rect() const;
        Prism bounding_prism() const;
        AABox bounding_aabox() const;

        void create_object(const Tete&);
        void update_object(const Tete&);
        void update_object_matrix(const Tete&);

        void receive_tete(const Tete&);

        virtual void render(RenderDimension, RenderPass);

    private:
        void init_matrix();
        void restore_matrix();
        void draw_text();
        void draw_text_billboard();
        void generate_texture();
        void generate_layout();

        std::string text_;
        ci::gl::Texture* text_texture_;
        ci::TextLayout* layout_;
        float text_size_;
    };
}