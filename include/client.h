
#pragma once

#include <list>

#include <renderer.h>
#include <objectController.h>
#include <identity.h>
#include <grids/interface.h>

class CinderApp;

namespace Atelier {
    class GenericNode; // Needed for testing
	class ClientNode; // The node-type interface with the system
	class TeteManager;
    class IdentityManager;
    class Object;

    typedef std::tr1::shared_ptr<ClientNode> ClientNodePtr;
    typedef std::tr1::shared_ptr<Object> ObjectPtr;

    class Client {
    public:
        Client(CinderApp*);

        void init();
        void update();
        void draw();

        static Client& instance();

        static const Identity& user_identity();
        static CinderApp& app();
        static Renderer& renderer();
        static const CameraNode& active_camera();
        static void set_active_camera(CameraNode*);
        static bool has_camera();

        static GenericNode* node;

        static Grids::Interface& grids_interface();

        // Registers object to get an update() call every frame refresh
        void operator+=(ObjectPtr);
        void operator-=(ObjectPtr);

		friend class ClientNode;

    private:
		Identity* create_user_identity();

        static CinderApp* app_;
        static Renderer renderer_;
        ObjectController object_controller_;
        static CameraNode* active_camera_;
        static Identity* user_identity_;
        static Grids::Interface grids_interface_;
        IdentityManager* identity_manager_;
        static Client* instance_;

		TeteManager* tete_manager_;
		ClientNodePtr client_node_;

        std::list<ObjectPtr> update_objects_;
        std::list<ObjectPtr>::const_iterator update_objects_it_;
    };
}
