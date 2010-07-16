
#include <objectController.h>
#include <identity.h>
#include <object.h>

namespace Atelier {
    ObjectController::ObjectController() {
        instance_ = this;
    }
    
    ObjectPtr ObjectController::get_object_from_id(const ID& in_id) const {
        std::map<ID, ObjectPtr>::const_iterator it = id_object_map_.find(in_id);

        return it == id_object_map_.end() ? NULL : 
            it->second;
    }

	ObjectPtr ObjectController::operator[](const ID& in_id) const {
		return get_object_from_id(in_id);
	}

    ObjectController* ObjectController::instance_;

    ObjectController& ObjectController::instance() {
        return *instance_;
    }

	void ObjectController::operator+=(ObjectPtr obj) {
		// Check if the object has already been registered
		id_object_map_it_ = id_object_map_.find(obj->id());

		if (id_object_map_it_ != id_object_map_.end())
			return;

		id_object_map_[obj->id()] = obj;
	}

	void ObjectController::operator-=(ObjectPtr obj) {
		id_object_map_it_ = id_object_map_.find(obj->id());

		if (id_object_map_it_ == id_object_map_.end())
			return;
		
		id_object_map_.erase(id_object_map_it_);
	}
}