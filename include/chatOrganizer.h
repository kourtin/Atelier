
#pragma once

// TODO: need to introduce locks to:
// position(), rotation(), scale(), bounding_rect()

#include <deque>

#include <boost/thread.hpp>

namespace Atelier {
    class ChatNode;
    class Object;
    class ChatMessageNode;

    typedef std::tr1::shared_ptr<ChatMessageNode> ChatMessageNodePtr;

    class ChatOrganizerWorker {
    public:
        ChatOrganizerWorker(std::deque<ChatMessageNodePtr> objects);

        void operator()();

    private:
        std::deque<ChatMessageNodePtr> objects_;
    };

    class ChatOrganizer {
    public:
        ChatOrganizer(ChatNode&);
        void init();

        void update();
        
    private:
        ChatNode& chat_node_;
        boost::thread worker_thread_;
    };
}