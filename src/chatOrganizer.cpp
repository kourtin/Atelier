
#include <deque>
#include <list>

#include <boost/thread.hpp>

#include <cinder/app/App.h>

#include <chatOrganizer.h>
#include <object.h>
#include <chatNode.h>
#include <client.h>
#include <basicApp.h>
#include <chatMessageNode.h>
#include <link.h>

namespace Atelier {
    //
    // ChatOrganizerWorker
    //
    ChatOrganizerWorker::ChatOrganizerWorker(ChatOrganizer& organizer,
        ChatMessageNodeList objects, int screen_width, int screen_height) : 
        organizer_(organizer) {
        objects_ = objects;
        screen_width_ = screen_width;
        screen_height_ = screen_height;

        // Note: if an object is in the 2D plane, it should be on the screen,
        // ie within width and height
    }

    void ChatOrganizerWorker::operator()() {
        organizer_.chat_energy_ = 0.0f;

        // Organize chats
        for (ChatMessageNodeList::const_iterator it = objects_.begin();
            it != objects_.end(); ++it) {
            doForces(*((*it).get())); // TODO: clean up this illedgible code
        }

        if (organizer_.chat_energy_ <= organizer_.chat_energy_threshold_)
            organizer_.running_ = false;
    }

    // Calculate the forces on a chat node based in its links
    void ChatOrganizerWorker::doForces(ChatMessageNode& node) {
        Vec3D force;

        // Sum all the forces pushing this item away
        for (ChatMessageNodeList::const_iterator it = objects_.begin();
            it != objects_.end(); ++it) {
            if (&node == (*it).get())
                continue;

            // Maybe I should be locking the position around this method...
            force += coulombRepulsion(node.position(), (*it)->position());
        }

        // take a snapshot of the node's current links
        std::list<LinkConstPtr> link_list = node.links();

        // Some nodes links could have more attractive force (eg more important)
        float attract_scale = 1.0f;

        // Sum all the forces from links pushing item away
        for (std::list<LinkConstPtr>::const_iterator it = link_list.begin();
            it != link_list.end(); ++it) {
            if ((*it)->actor().object() == NULL)
                continue;

            // Check if this object is in the chat
            if (!node_in_chat((*it)->actor().object()))
                continue;

            force += hookeAttraction(node.position(), 
                (*it)->actor().object()->position(),
                organizer_.rest_distance_ / attract_scale);
        }

        // check for NaNs
        if (force != force)
            force = Vec3D();

        if(force.x > organizer_.max_velocity_)
            force.x = organizer_.max_velocity_;
        else if(force.x < -organizer_.max_velocity_)
            force.x = -organizer_.max_velocity_;
        if(force.y > organizer_.max_velocity_)
            force.y = organizer_.max_velocity_;
        else if(force.y < -organizer_.max_velocity_)
            force.y = -organizer_.max_velocity_;

        force.z = 0.0f; // Chats are in 2D atm

        node.set_velocity(force*organizer_.damping_);
        organizer_.chat_energy_ += force.length();
    }

    Vec3D ChatOrganizerWorker::coulombRepulsion(Vec3D node_1_pos, Vec3D node_2_pos) {
        float dx = node_1_pos.x - node_2_pos.x;
        float dy = node_1_pos.y - node_2_pos.y;

        float r = sqrt(dx*dx + dy*dy);

        return Vec3D( (dx * organizer_.repulse_weight_) / r,
            (dy * organizer_.repulse_weight_) / r, 0.0f);
    }

    // Notice that the direction is flipped
    Vec3D ChatOrganizerWorker::hookeAttraction(Vec3D node_1_pos, Vec3D node_2_pos,
        float rest_distance) {
        float dx = node_2_pos.x - node_1_pos.x;
        float dy = node_2_pos.y - node_1_pos.y;

        float r = sqrt(dx*dx + dy*dy);
        float displacement = r - rest_distance;

        return Vec3D((dx/r)*displacement*organizer_.attract_weight_, 
            (dy/r)*displacement*organizer_.attract_weight_, 0.0);                   
    }

    // TODO: fix this slow function
    bool ChatOrganizerWorker::node_in_chat(ObjectConstPtr object) {
        for (ChatMessageNodeList::const_iterator it = objects_.begin();
            it != objects_.end(); ++it) {
            if ((*it)->id() == object->id())
                return true;
        }
        return false;
    }

    //
    // ChatOrganizer
    //
    ChatOrganizer::ChatOrganizer(ChatNode& node) : chat_node_(node) {
        running_ = false;

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
        if (running_ == false)
            return;

        if (chat_node_.chat_messages_.empty())
            return;

        for (ChatMessageNodeList::const_iterator it = chat_node_.chat_messages_.begin();
            it != chat_node_.chat_messages_.end(); ++it) {
            (*it)->update_velocity();
        }

        if (worker_thread_.joinable())
            return;

        ChatOrganizerWorker worker(*this, chat_node_.chat_messages_,
            Client::app().getDisplay().getWidth(),
            Client::app().getDisplay().getHeight());

        worker_thread_ = boost::thread(worker);
    }

    bool ChatOrganizer::within_screen_bounds(Vec3D pos, Vec3D scl, Rect in_bounds) {
        int min_x = 0;
        int max_x = Client::app().getDisplay().getWidth();
        int min_y = 0;
        int max_y = Client::app().getDisplay().getHeight();

        ci::Vec2f pos2d = ci::Vec2f(pos.x, pos.y);
        Rect bounds = in_bounds * scl.x;
        bounds += pos2d;
        
        if (bounds.getX1() < min_x || bounds.getX1() > max_x || 
            bounds.getX2() < min_x || bounds.getX2() > max_x ||
            bounds.getY1() < min_y || bounds.getY1() > max_y ||
            bounds.getY2() < min_y || bounds.getY2() > max_y)
            return false;

        return true;
    }

    void ChatOrganizer::start() {
        running_ = true;
    }
}