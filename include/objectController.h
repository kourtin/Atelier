
#pragma once

#include <map>

#include "define.h"

namespace Atelier {
    class Object;

    class ObjectController {
    public:
        ObjectController();
        static ObjectController* instance();

        Object* get_object_from_id(const ID&) const;

    private:
        static ObjectController* instance_;

        std::map<ID, Object*> id_object_map_;
        std::map<Object*, ID> object_id_map_;
    };
}