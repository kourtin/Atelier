
#pragma once

#include <cinder/Text.h>
#include <cinder/gl/Texture.h>

#include <node2D.h>
#include <link.h>

namespace Atelier {
    class Link;

    class ChatMessageNode : public Node2D {
    public:
        ChatMessageNode(const ID&);
        virtual ~ChatMessageNode();

        static void request_create(const ID& req_id, const Identity& chat_ident, 
            std::string initial_text, const std::deque<LinkPtr>& links);
        static void request_update_text(const Identity& node_ident, 
            const std::string&);
        static void request_finish_update(const Identity& node_ident, 
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

        const InteractItem& container() const;

    private:
        void init_matrix();
        void restore_matrix();
        void draw_text();
        void draw_text_billboard();
        void generate_texture();
        void generate_layout();

        std::string text_;

        typedef std::tr1::shared_ptr<ci::gl::Texture> TexturePtr;
        typedef std::tr1::shared_ptr<ci::TextLayout> LayoutPtr;

        TexturePtr text_texture_;
        LayoutPtr layout_;
        float text_size_;
        std::tr1::shared_ptr<const InteractItem> container_; // The UserNode
        mutable boost::recursive_mutex text_texture_mutex_;

        typedef boost::recursive_mutex::scoped_lock ScopedLock;
    };

    typedef std::tr1::shared_ptr<ChatMessageNode> ChatMessageNodePtr;
}