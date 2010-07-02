
#include <assert.h>

#include <map>
#include <iterator>
#include <exception>

#include <identity.h>
#include <objectController.h>
#include <object.h>
#include <identityManager.h>

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
        object_ = ObjectController::instance().get_object_from_id(new_id);
        is_local_ = object_ != NULL;
        is_valid_ = true;
    }

    Identity::Identity(const ID& new_id, Object* object) {
        id_ = new_id;
        object_ = object;
        is_local_ = object_ != NULL;
        is_valid_ = true;
    }

    Identity::~Identity() {
    }

    // See Meyers Item #3 for explanation of this code
    Object* Identity::object() {
        return const_cast<Object*>(static_cast<const Identity&>(*this).object());
    }

    const Object* Identity::object() const {
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

        Object* temp_object = ObjectController::instance().get_object_from_id(new_id);
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

    bool Identity::operator<(const Identity& ident) const {
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

    bool Identity::operator==(const Identity& ident) const {
        if (id_ != ident.id_)
            return false;

        assert(object_ == ident.object_);
        return true;
    }

	bool Identity::operator!=(const Identity& ident) const {
		return !(*this == ident);
	}

    void Identity::set_name(const std::string& new_name) {
        name_ = new_name;
    }

    std::string& Identity::name() {
        return name_;
    }

    const std::string& Identity::name() const {
        return name_;
    }

    const ID& Identity::current_room() const {
        return current_room_;
    }

    void Identity::set_current_room(const ID& rm) {
        current_room_ = rm;
    }

    ///////////////////////
    // Static Utility stuff
    ///////////////////////
    
    void Identity::register_identity(const Identity& ident) {
		IdentityManager::instance().id_identity_ptr_map_iterator_ = 
			IdentityManager::instance().id_identity_ptr_map_.find(ident.id());

        if (IdentityManager::instance().id_identity_ptr_map_iterator_ == 
			IdentityManager::instance().id_identity_ptr_map_.end())
            IdentityManager::instance().id_identity_ptr_map_[ident.id()] = &ident;
    }

    const Identity* Identity::get_identity_from_value(const Value& val) {
        if (val["id"].empty())
            return NULL;

        ID val_id = val["id"].asString();
        return get_identity_from_id(val_id);
    }

    // Maybe instead this should throw an exception?
    const Identity* Identity::get_identity_from_id(const ID& in_id) {
        std::map<const ID, const Identity*>& temp_map = 
            IdentityManager::instance().id_identity_ptr_map_;

        IdentityManager::instance().id_identity_ptr_map_iterator_ = 
			IdentityManager::instance().id_identity_ptr_map_.find(in_id);

        if (IdentityManager::instance().id_identity_ptr_map_iterator_ == 
			IdentityManager::instance().id_identity_ptr_map_.end())
            return NULL;

        return IdentityManager::instance().id_identity_ptr_map_iterator_->second;
    }

    Identity* Identity::create_identity(ID& in_id, Object* obj) {
        if (in_id.empty())
            return NULL;

        Identity* ident = new Identity(in_id, obj);

        register_identity(*ident);

        return ident;
    }

    Identity* Identity::create_identity(const Value& val) {
        if (val["id"].empty())
            return NULL;
        ID owner_id  = val["id"].asString();
        // This is called when there's no local object, 
        Identity* ident = new Identity(owner_id);

        if (!val["name"].empty())
            ident->set_name(val["name"].asString());

        // What about other attributes such as color, font, hat size?
        // This should be transmitted as the creation, etc, of a 
        // separate object.

        register_identity(*ident);

        return ident;
    }
}