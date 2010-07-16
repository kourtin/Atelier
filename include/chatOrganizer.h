
#pragma once

// TODO: need to introduce locks to:
// position(), rotation(), scale(), bounding_rect()

namespace Atelier {
    class ChatNode;
    class Object;

    class ChatOrganizerWorker {
    public:
        ChatOrganizerWorker(std::vector<Object*> objects);

        void operator()();

    private:
        std::vector<Object*> objects_;
    };

    class ChatOrganizer {
    public:
        ChatOrganizer(ChatNode&);
        void init();

        void update();
        
    private:
        ChatNode& chat_node_;

    };
}