
#pragma once

#include "define.h"

// Tetes v Values. Tetes are events, nugets of information 
// passed between clients, or within a client (say recorded 
// and later played back to restore a state). A value is a
// a lightweight data stream (JSON at the moment).

namespace Atelier {
    class Identity;
    class Tete;

    class ObjectInterface {
    public:
        virtual const ID& id() = 0;

        virtual const ID& owner() = 0;

		virtual std::string type() = 0;

        virtual Vec3D position() = 0;
        virtual Vec3D rotation() = 0;
        virtual Vec3D scale() = 0;

        virtual void set_position(Vec3D) = 0;
        virtual void set_rotation(Vec3D) = 0;
        virtual void set_scale(Vec3D) = 0;

        virtual void create_object(const Tete&) = 0;
        virtual void update_object(const Tete&) = 0;
        virtual void update_object_matrix(const Tete&) = 0;

        virtual void receive_tete(const Tete&) = 0;

        // Should this receive some sort of event?
        // Activate is an abstracted way to activate a node,
        // it can be generated from a UI event, Network event, etc
        virtual void activate(const Identity&) = 0;
    };
}