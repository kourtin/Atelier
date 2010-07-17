
#include <deque>

#include <boost/thread.hpp>

#include <cinder/app/App.h>

#include <chatOrganizer.h>
#include <object.h>
#include <chatNode.h>
#include <client.h>
#include <basicApp.h>

namespace Atelier {
    //
    // ChatOrganizerWorker
    //
    ChatOrganizerWorker::ChatOrganizerWorker(ChatOrganizer& organizer,
        std::deque<ChatMessageNodePtr> objects, int screen_width, 
        int screen_height) : organizer_(organizer) {
        objects_ = objects;
        screen_width_ = screen_width;
        screen_height_ = screen_height;

        // Note: if an object is in the 2D plane, it should be on the screen,
        // ie within width and height
    }

    void ChatOrganizerWorker::operator()() {
        // Organize chats
        for (std::deque<ChatMessageNodePtr>::const_iterator it = objects_.begin();
            it != objects_.end(); ++it) {
            doForces(*((*it).get())); // Lol C++
        }
    }

    // Calculate the forces on a chat node based in its links
    void ChatOrganizerWorker::doForces(ChatMessageNode& node) {

    }

    Vec3D ChatOrganizerWorker::coulombRepulsion(Vec3D node_1_pos, Vec3D node_2_pos) {
        float dx = node_1_pos.x - node_2_pos.x;
        float dy = node_1_pos.y - node_2_pos.y;

        float r = sqrt(dx*dx + dy*dy);

        return Vec3D( (dx * organizer_.repulse_weight_) / r,
            (dy * organizer_.repulse_weight_) / r, 0.0);
    }

    // Notice that the direction is flipped
    Vec3D ChatOrganizerWorker::hookeAttraction(Vec3D node_1_pos, Vec3D node_2_pos,
        float rest_distance) {
        float dx = node_2_pos.x -
               node_1_pos.x;
        float dy = node_2_pos.y -
               node_1_pos.y;

        float r = sqrt(dx*dx + dy*dy);
        float displacement = r - rest_distance;

        return Vec3D((dx/r)*displacement*organizer_.attract_weight_, 
            (dy/r)*displacement*organizer_.attract_weight_, 0.0);                   
    }

    //
    // ChatOrganizer
    //
    ChatOrganizer::ChatOrganizer(ChatNode& node) : chat_node_(node) {
        chat_energy_ = 0.0f;
        chat_energy_threshold_ = 0.6f;
        min_velocity_ = 0.1f;
        max_velocity_ = 10.f;
        damping_ = 0.02f;
        // For attract_weight_, larger is larger, faster, stronger
        attract_weight_ = 10.0;
        // For repulse_weight_, larger is larger, faster, stronger
        repulse_weight_ = 55.0;
        rest_distance_ = 50.0f;
    }

    void ChatOrganizer::init() {
        // Nothnig here
    }

    void ChatOrganizer::update() {
        if (worker_thread_.joinable())
            return;

        ChatOrganizerWorker worker(*this, chat_node_.chat_messages_,
            Client::app().getDisplay().getWidth(),
            Client::app().getDisplay().getHeight());

        worker_thread_ = boost::thread(worker);
    }
}