
#pragma once

#include <map>

#include <define.h>

namespace Atelier {
    class Object;
    class Identity;

    class ObjectController {
    public:
        ObjectController();
        static ObjectController& instance();

        Object* get_object_from_id(const ID&) const;
		Object* operator[](const ID&) const;

		void operator+=(Object*);
		void operator-=(Object*);

    private:
        static ObjectController* instance_;

        std::map<ID, Object*> id_object_map_;

		std::map<ID, Object*>::const_iterator id_object_map_it_;
    };
}