
#include <algorithm>

#include <teteManager.h>
#include <object.h>

namespace Atelier {
	TeteManager::TeteManager() {
		tete_manager_ = this;
	}

	TeteManager* TeteManager::tete_manager_;

	TeteManager& TeteManager::instance() {
		return *tete_manager_;
	}

	// this probably shouldn't block
	void TeteManager::update() {
		while (!tete_queue_.empty()) {
			const Tete& tete = *(tete_queue_.front());
			
			for (std::list<Object*>::const_iterator it = registered_objects_.begin();
				it != registered_objects_.end(); ++it) {
				(*it)->receive_tete(tete);
			}
			tete_queue_.pop();
		}
	}

	void TeteManager::operator+=(Object* obj) {
		registered_objects_it_ = find(registered_objects_.begin(),
			registered_objects_.end(), obj);

		if (registered_objects_it_ == registered_objects_.end())
			registered_objects_.push_back(obj);
	}

	void TeteManager::operator-=(Object* obj) {
		registered_objects_it_ = find(registered_objects_.begin(),
			registered_objects_.end(), obj);

		if (registered_objects_it_ == registered_objects_.end())
			return;

		registered_objects_.erase(registered_objects_it_);
	}
}
