
#include <boost/uuid/uuid_io.hpp>

#include "client.h"
#include "basicApp.h"
#include "genericNode.h"
#include "spaceGraphics.h"  
#include "cameraNode.h"
#include <utility.h>

namespace Atelier {
    Client::Client(CinderApp* app) {
        // WARNING: do not dereference app in the ctor!
        app_ = app;
        active_camera_ = NULL;
    }

    GenericNode* Client::node;

    void Client::init() {
		set_user_identity(create_user_identity());

        grids_interface_.init();
        grids_interface_.connect_to_node();
		grids_interface_.request_list_rooms();

		enter_default_room();

        active_camera_ = new CameraNode("lol_dongs2");
        renderer_.set_camera(active_camera_);

        node = new GenericNode("lolhi");

        // NOTE: this method binds GL textures, and therefore MUST be in the setup() method.
        node->init();
        SpaceGraphics* static_item = new Atelier::SpaceGraphics();
   
        renderer_.add_item(node);
        renderer_.add_static_item(static_item);
        renderer_.init();
    }

    void Client::update() {
        grids_interface_.update();

        renderer_.update();
        renderer_.call_camera_matrix();
    }

    void Client::draw() {
        renderer_.render();
    }

    CinderApp* Client::app_;
    Renderer Client::renderer_;
    CameraNode* Client::active_camera_;
    Identity Client::user_identity_;
    Grids::Interface Client::grids_interface_;

    const Identity& Client::user_identity() {
        return user_identity_;
    }

    void Client::set_user_identity(const Identity& ident) {
        user_identity_ = ident;
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

	Identity Client::create_user_identity() {
		// This should be loaded from a sqlite db or something
        Identity ident(Utility::create_uuid());
        ident.set_name("Patrick Tierney");
        return ident;
	}

	void Client::enter_default_room() {

	}
}