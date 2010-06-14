
#include <assert.h>

#include "identity.h"
#include "objectController.h"
#include "object.h"

// Notes on asserts: It's very important that objcts with the same ids have the same pointers,
// and vice versa. Any inconsistancies, say from the network, should be resolved there
// before creating a new Atelier::Identity.

namespace Atelier {
    Identity::Identity() {
        is_valid_ = false;
        is_local_ = false;
        object_ = NULL;
    }

    Identity::Identity(const ID& new_id) {
        id_ = new_id;
        object_ = ObjectController::instance()->get_object_from_id(new_id);
        object_ != NULL ? is_local_ = true : is_local_ = false;
        is_valid_ = true;
    }

    Identity::Identity(const ID& new_id, Object* object) {
        id_ = new_id;
        object_ = object;
        assert(object);
        is_valid_ = true;
        is_local_ = true;
    }

    Identity::~Identity() {
    }

    Object* Identity::object() const {
        return is_valid_ ? object_ : NULL;
    }

    void Identity::set_object(Object* obj) {
        assert(obj);
        assert(id_ == obj->id());
        object_ = obj;
        is_valid_ = true;
        is_local_ = true;
    }

    const ID& Identity::id() const {
        return id_;
    }

    void Identity::set_id(const ID& new_id) {
        id_ = new_id;

        Object* temp_object = ObjectController::instance()->get_object_from_id(new_id);
        if(temp_object && object_) 
            assert(temp_object == object_);

        temp_object ? is_local_ = true : is_local_ = false;
        is_valid_ = true;
    }

    bool Identity::is_valid() const {
        return is_valid_;
    }

    bool Identity::is_local() const {
        return is_local_;
    }

    Identity& Identity::operator=(const Identity& ident) {
        object_ = ident.object_;
        id_ = ident.id_;

        return *this;
    }

    bool Identity::operator<(const Identity& ident) {
        std::string first = id_;
        std::string second = ident.id_;

        if (first == second)
            return false;

        for (unsigned int i = 0; (i < first.length()) && (i < second.length()); ++i) {
            if (tolower (first[i]) < tolower (second[i])) 
                return true;
            else if (tolower (first[i]) > tolower (second[i])) 
                return false;
        }

        return false;
    }

    bool Identity::operator==(const Identity& ident) {
        if (id_ != ident.id_)
            return false;

        assert(object_ == ident.object_);
        return true;
    }

}