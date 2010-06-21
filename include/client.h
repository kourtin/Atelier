
#pragma once

#include <renderer.h>
#include <objectController.h>
#include <identity.h>
#include <grids/interface.h>

class CinderApp;

namespace Atelier {
    class GenericNode; // Needed for testing

    class Client {
    public:
        Client(CinderApp*);

        void init();
        void update();
        void draw();

        static const Identity& user_identity();
        static void set_user_identity(const Identity&);
        static CinderApp& app();
        static Renderer& renderer();
        static const CameraNode& active_camera();
        static void set_active_camera(CameraNode*);
        static bool has_camera();

        static GenericNode* node;

        static Grids::Interface& grids_interface();

    private:
        static CinderApp* app_;
        static Renderer renderer_;
        ObjectController object_controller_;
        static CameraNode* active_camera_;
        static Identity user_identity_;
        static Grids::Interface grids_interface_;
    };
}