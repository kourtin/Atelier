
#include <deque>

#include <boost/thread.hpp>

#include <chatOrganizer.h>
#include <object.h>
#include <chatNode.h>

namespace Atelier {
    ChatOrganizerWorker::ChatOrganizerWorker(std::deque<ChatMessageNodePtr> objects) {
        objects_ = objects;
    }

    void ChatOrganizerWorker::operator()() {
        // Organize chats
    }

    ChatOrganizer::ChatOrganizer(ChatNode& node) : chat_node_(node) {

    }

    void ChatOrganizer::init() {

    }

    void ChatOrganizer::update() {
        if (worker_thread_.joinable())
            return;

        ChatOrganizerWorker worker(chat_node_.chat_messages_);

        worker_thread_ = boost::thread(worker);
    }
}