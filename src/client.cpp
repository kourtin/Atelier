
#include <algorithm>
#include <list>

#include <boost/uuid/uuid_io.hpp>

#include <client.h>
#include <basicApp.h>
#include <genericNode.h>
#include <spaceGraphics.h> 
#include <cameraNode.h>
#include <utility.h>
#include <teteManager.h>
#include <clientNode.h>
#include <userNode.h>
#include <identityManager.h>
#include <chatMessageNode.h>

namespace Atelier {
    Client::Client(CinderApp* app) {
        // WARNING: do not dereference app in the ctor!
        app_ = app;
        active_camera_ = NULL;
        instance_ = this;
    }

    GenericNode* Client::node;

    void Client::init() {
        identity_manager_ = new IdentityManager();

		user_identity_ = create_user_identity(); // Should load from disk in future

		tete_manager_ = new TeteManager();
		// Note that I don't request this node from the network. All other nodes should be
		client_node_ = ClientNodePtr(new ClientNode(Utility::create_uuid()));

		(*tete_manager_) += client_node_; // Register the node to receive Tetes

        grids_interface_.init();
        grids_interface_.connect_to_node();
		grids_interface_.request_list_rooms();

        SpaceGraphics* static_item = new Atelier::SpaceGraphics();
        renderer_.add_static_item(static_item);

        renderer_.init();

        //ChatMessageNode* m_node = new ChatMessageNode("lolhi");
    }

    void Client::update() {
        grids_interface_.update();

		TeteManager::instance().update();

        for (std::list<ObjectPtr>::const_iterator it = update_objects_.begin();
            it != update_objects_.end(); ++it) {
            (*it)->update();
        }

        renderer_.update();
        renderer_.call_camera_matrix();
    }

    void Client::draw() {
        renderer_.render();
    }

    Client& Client::instance() {
        return *instance_;
    }

    CinderApp* Client::app_;
    Renderer Client::renderer_;
    CameraNode* Client::active_camera_;
    Identity* Client::user_identity_;
    Grids::Interface Client::grids_interface_;
    Client* Client::instance_;

    const Identity& Client::user_identity() {
        return *user_identity_;
    }

    CinderApp& Client::app() {
        return *app_;
    }

    Renderer& Client::renderer() {
        return renderer_;
    }

    Grids::Interface& Client::grids_interface() {
        return grids_interface_;
    }

    const CameraNode& Client::active_camera() {
        return *active_camera_;
    }

    void Client::set_active_camera(CameraNode* cam) {
        active_camera_ = cam;
    }

    bool Client::has_camera() {
        return active_camera_ ? true : false;
    }

	Identity* Client::create_user_identity() {
		// This should be loaded from a sqlite db or something
        Identity* ident = Identity::create_identity(Utility::create_uuid(), NULL);
        ident->set_name("Patrick Tierney");
        return ident;
	}

    void Client::operator+=(ObjectPtr obj) {
        update_objects_it_ = find(update_objects_.begin(),
            update_objects_.end(), obj);

        if (update_objects_it_ == update_objects_.end())
            update_objects_.push_back(obj);
    }

    void Client::operator-=(ObjectPtr obj) {
        update_objects_it_ = find(update_objects_.begin(),
            update_objects_.end(), obj);

        if (update_objects_it_ == update_objects_.end())
            return;

        update_objects_.erase(update_objects_it_);
    }
}
