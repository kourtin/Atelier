
#pragma once

#include <deque>

#include <object.h>
#include <gridsNetworkItem.h>
#include <cinder2DInteractItem.h>

namespace Atelier {
    class ChatMessageNode;

    class ChatNode : public Object, public GridsNetworkItem,
        public Cinder2DInteractItem {
    public:
        ChatNode(const ID&);
        virtual ~ChatNode();

        std::string type() { return "ChatNode"; }

        static void request_create();

        Rect bounding_rect() const;
        const InteractItem& container() const;

        void create_object(const Tete&);
        void update_object(const Tete&);
        void update_object_matrix(const Tete&);

        void receive_tete(const Tete&);
        void activate(const Identity&);

        bool keyDown(ci::app::KeyEvent);

    private:
        void message_start();
        void message_updated();
        void message_entered();

        std::deque<ChatMessageNode*> chat_messages_;
        const InteractItem* container_; // The UserNode
        std::string text_buffer_;
        ChatMessageNode* active_node_; // the node you're typing into
    };
}