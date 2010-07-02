
#pragma once

#include <map>

#include <define.h>

namespace Atelier {
	class Identity;

	class IdentityManager {
    public:
		IdentityManager();

		static IdentityManager& instance();

		friend class Identity;

	private:
        std::map<const ID, const Identity*> id_identity_ptr_map_;
        std::map<const ID, const Identity*>::const_iterator
            id_identity_ptr_map_iterator_;

		static IdentityManager* instance_;
	};
}