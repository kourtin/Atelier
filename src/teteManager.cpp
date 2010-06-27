
#include <teteManager.h>

namespace Atelier {
	TeteManager::TeteManager() {
		tete_manager_ = this;
	}

	TeteManager* TeteManager::tete_manager_;

	TeteManager& TeteManager::instance() {
		return *tete_manager_;
	}

	void TeteManager::update() {
		// probably shouldn't block, btw
	}
}
