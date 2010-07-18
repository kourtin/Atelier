
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
    typedef std::deque<ChatMessageNodePtr> ChatMessageNodeList;
    typedef std::tr1::shared_ptr<Object> ObjectPtr;
    typedef std::tr1::shared_ptr<const Object> ObjectConstPtr;

    class ChatOrganizerWorker {
    public:
        ChatOrganizerWorker(boost::mutex&, ChatOrganizer&,
            ChatMessageNodeList objects,
            int screen_width, int screen_height);

        void operator()();

    private:
        void doForces(ChatMessageNode&);
        Vec3D coulombRepulsion(Vec3D pos1, Vec3D pos2);
        Vec3D hookeAttraction(Vec3D pos1, Vec3D pos2, float rest_dist);
        bool node_in_chat(ObjectConstPtr);

        ChatOrganizer& organizer_;
        ChatMessageNodeList objects_;
        int screen_width_;
        int screen_height_;
        boost::mutex& running_mutex_;
    };

    class ChatOrganizer {
        friend class ChatOrganizerWorker;

    public:
        ChatOrganizer(ChatNode&);
        void init();

        void update();
        void start();

        static bool within_screen_bounds(Vec3D pos, Vec3D scl, Rect bounds);
        
    private:
        ChatNode& chat_node_;
        boost::thread worker_thread_;

        // The organizer only needs to run while the nodes are in a high energy state
        bool running_;

        float chat_energy_;
        float chat_energy_threshold_;
        float min_velocity_;
        float max_velocity_;
        float damping_;
        float attract_weight_;
        float repulse_weight_;
        float rest_distance_;
        float attract_scale_;

        boost::mutex running_mutex_;
    };

    typedef std::tr1::shared_ptr<ChatOrganizer> ChatOrganizerPtr;
}