
#pragma once

#include <list>

#include "define.h"

// Tetes v Values. Tetes are events, nugets of information 
// passed between clients, or within a client (say recorded 
// and later played back to restore a state). A value is a
// a lightweight data stream (JSON at the moment).

namespace Atelier {
    class Identity;
    class Tete;
    class Link;

    class ObjectInterface {
    public:
        virtual const ID& id() = 0;

		virtual std::list<const Link*>& links() = 0;
        virtual const std::list<const Link*>& links() const = 0;

		virtual std::string type() = 0;

        virtual Vec3D position() const = 0;
        virtual Vec3D rotation() const = 0;
        virtual Vec3D scale() const = 0;

        virtual void set_position(Vec3D) = 0;
        virtual void set_rotation(Vec3D) = 0;
        virtual void set_scale(Vec3D) = 0;

        // These methods are called when a create, update, or update matrix event
        // is received and intended for this object
        virtual void create_object(const Tete&) = 0;
        virtual void update_object(const Tete&) = 0;
        virtual void update_object_matrix(const Tete&) = 0;

        // An object can register itself to receive all Tetes arriving on 
        // the network. The events are dumped here
        virtual void receive_tete(const Tete&) = 0;

        // Should this receive some sort of event?
        // Activate is an abstracted way to activate a node,
        // it can be generated from a UI event, Network event, etc
        virtual void activate(const Identity&) = 0;
    };
}