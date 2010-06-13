
#include "client.h"
#include "basicApp.h"
#include "genericNode.h"
#include "spaceGraphics.h"  
#include "cameraNode.h"

namespace Atelier {
    Client::Client(CinderApp* app) {
        app_ = app;
    }

    void Client::setup() {
        renderer_.set_camera(new CameraNode("lol_dongs2"));

        GenericNode* generic_node_ = new GenericNode("lolhi");
        // NOTE: this method binds GL textures, and therefore MUST be in the setup() method.
        generic_node_->init();
        SpaceGraphics* static_item = new Atelier::SpaceGraphics();
   
        renderer_.add_item(generic_node_);
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

    CinderApp& Client::app() {
        return *app_;
    }

    Renderer& Client::renderer() {
        return renderer_;
    }

}