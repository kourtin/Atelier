
#pragma once

#include <map>

#include <define.h>
#include <object.h> // For ObjectPtr

namespace Atelier {
    class Object;
    class Identity;

    class ObjectController {
    public:
        ObjectController();
        static ObjectController& instance();

        ObjectPtr get_object_from_id(const ID&) const;
		ObjectPtr operator[](const ID&) const;

		void operator+=(ObjectPtr);
		void operator-=(ObjectPtr);

    private:
        static ObjectController* instance_;

        std::map<ID, ObjectPtr> id_object_map_;
		std::map<ID, ObjectPtr>::const_iterator id_object_map_it_;
    };
}