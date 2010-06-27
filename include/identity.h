
/* 
    What is an Identity?
    All the objects in this program exist on both the network and 
    on the client program[s], and are referenced by a uuid and 
    various cryptographic mojo that allows you to varify an object is 
    who it says it is.

    In other words, if you desire to perform an action, or are 
    notified of an action to an object, it is both pointless and
    dangerous to perform that action on the local object reference or 
    pointer. Instead the action should be performed, requested, etc
    on the identity of the object. This guarantees that:
        * you are in the proper network proximity to the object
        * you have cryptographic permissions to communicate with the object
        * (probably more...)
    Manipulating objects is based on a series of requests to a server,
    and actions take place when the object is bounced back.
*/

/*
    TODO: 
        * Add cryptographic mojo, maybe break the object up into 
        cryptographic and non-cryptographic.
*/


#pragma once

#include <map>

#include "define.h"

namespace Atelier {
    class Object;

    class Identity {
    public:
        Identity();
        Identity(const ID&);
        Identity(const ID&, Object*);
        ~Identity();

        Object* object() const;
        void set_object(Object*);
        const ID& id() const;
        void set_id(const ID&);
        const ID& current_room() const;
        void set_current_room(const ID&);

        bool is_valid() const;
        bool is_local() const;

        // Names are the human readable representation of the identity
        void set_name(const std::string&);
        std::string& name();
        const std::string& name() const;

        Identity& operator=(const Identity& ident);
        bool operator<(const Identity& ident); // uses the id string
        bool operator==(const Identity& ident);

    private:
        Object* object_;
        ID id_;
        ID current_room_;
        std::string name_;
        bool is_valid_;
        bool is_local_;

    // Identity utility stuff
    public:
        static void register_identity(const Identity&);
        static const Identity* get_identity_from_value(const Value&);
        static const Identity* get_identity_from_id(const ID&);
        // Probably should overload this 
        static Identity* create_identity(ID&, Object*);
        // Use this if you receive a message but can't find the 
        // Identity of the creator.
        static Identity* create_identity(const Value&);
         
    private:
		// TODO: remove these
        static std::map<const ID, const Identity*> id_identity_ptr_map_;
        static std::map<const ID, const Identity*>::const_iterator
            id_identity_ptr_map_iterator_;
    };
}