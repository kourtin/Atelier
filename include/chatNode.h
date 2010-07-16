
#pragma once

#include <deque>

#include <object.h>
#include <gridsNetworkItem.h>
#include <cinder2DInteractItem.h>

namespace Atelier {
    class ChatMessageNode;

    typedef std::tr1::shared_ptr<ChatMessageNode> ChatMessageNodePtr;

    class ChatNode : public Object, public GridsNetworkItem,
        public Cinder2DInteractItem {
        friend class ChatOrganizer;
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

        std::deque<ChatMessageNodePtr> chat_messages_;
        std::tr1::shared_ptr<const InteractItem> container_; // The UserNode
        std::string text_buffer_;
        ChatMessageNode* active_node_; // the node you're typing into
        Identity* active_node_identity_;
        bool unsent_chars_;
    };
}