
#pragma once

// TODO: need to introduce locks to:
// position(), rotation(), scale(), bounding_rect()

#include <deque>

#include <boost/thread.hpp>

#include <define.h>

namespace Atelier {
    class ChatNode;
    class Object;
    class ChatMessageNode;
    class ChatOrganizer;

    typedef std::tr1::shared_ptr<ChatMessageNode> ChatMessageNodePtr;

    class ChatOrganizerWorker {
    public:
        ChatOrganizerWorker(ChatOrganizer&,
            std::deque<ChatMessageNodePtr> objects,
            int screen_width, int screen_height);

        void operator()();

    private:
        void doForces(ChatMessageNode&);
        Vec3D coulombRepulsion(Vec3D pos1, Vec3D pos2);
        Vec3D hookeAttraction(Vec3D pos1, Vec3D pos2, float rest_dist);

        ChatOrganizer& organizer_;
        std::deque<ChatMessageNodePtr> objects_;
        int screen_width_;
        int screen_height_;
    };

    class ChatOrganizer {
        friend class ChatOrganizerWorker;

    public:
        ChatOrganizer(ChatNode&);
        void init();

        void update();
        
    private:
        ChatNode& chat_node_;
        boost::thread worker_thread_;

        float chat_energy_;
        float chat_energy_threshold_;
        float min_velocity_;
        float max_velocity_;
        float damping_;
        float attract_weight_;
        float repulse_weight_;
        float rest_distance_;
    };

    typedef std::tr1::shared_ptr<ChatOrganizer> ChatOrganizerPtr;
}