
#pragma once

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
        std::string name_;
        bool is_valid_;
        bool is_local_;
    };
}