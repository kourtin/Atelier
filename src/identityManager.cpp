
#include <identityManager.h>

namespace Atelier {
	IdentityManager::IdentityManager() {
		instance_ = this;
	}

	IdentityManager* IdentityManager::instance_;

	IdentityManager& IdentityManager::instance() {
		return *instance_;
	}
}