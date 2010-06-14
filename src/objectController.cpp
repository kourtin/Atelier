
#include "objectController.h"

namespace Atelier {
    ObjectController::ObjectController() {
        instance_ = this;
    }
    
    Object* ObjectController::get_object_from_id(const ID& in_id) const {
        std::map<ID, Object*>::const_iterator it = id_object_map_.find(in_id);

        return it == id_object_map_.end() ? NULL : 
            it->second;
    }

    ObjectController* ObjectController::instance_;

    ObjectController* ObjectController::instance() {
        return instance_;
    }
}