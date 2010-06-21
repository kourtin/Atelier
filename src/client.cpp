
#include "client.h"
#include "basicApp.h"
#include "genericNode.h"
#include "spaceGraphics.h"  
#include "cameraNode.h"

namespace Atelier {
    Client::Client(CinderApp* app) {
        app_ = app;
        active_camera_ = NULL;
    }

    GenericNode* Client::node;

    void Client::init() {
        Identity* ident = new Identity("mrpatricktierney");
        set_user_identity(*ident);

        grids_interface_.init();
        grids_interface_.connect_to_node();

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
}