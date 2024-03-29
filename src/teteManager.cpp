
#include <algorithm>

#include <cinder/app/App.h>

#include <teteManager.h>
#include <object.h>
#include <tete.h>
#include <objectController.h>

namespace Atelier {
	TeteManager::TeteManager() {
		tete_manager_ = this;
	}

	TeteManager* TeteManager::tete_manager_;

	TeteManager& TeteManager::instance() {
		return *tete_manager_;
	}

	// this probably shouldn't block, or should run in a background thread
	void TeteManager::update() {
        // receive_tete may try to modify registered_objects_. Guard against it
        std::list<ObjectPtr> reg_copy = registered_objects_;

		while (!tete_queue_.empty()) {
			const Tete& tete = *(tete_queue_.front());
			
			if (registered_objects_.empty()) {
				ci::app::console() << "WARNING: No objects registered, droping message:" 
					<< std::endl << tete.value().toStyledString() << std::endl;
			}
            
            // The object being updated should always receive the update event
            if (tete.type() == Tete::UPDATE)
                distribute_update(tete);

			for (std::list<ObjectPtr>::const_iterator it = reg_copy.begin();
				it != reg_copy.end(); ++it) {
				(*it)->receive_tete(tete);
			}
			tete_queue_.pop();
		}
	}

    void TeteManager::distribute_update(const Tete& tete) {
        ObjectPtr obj = ObjectController::instance().get_object_from_id(tete.id());

        if(obj.get() == NULL)
            return;

        obj->update_object(tete);
    }

	void TeteManager::operator+=(ObjectPtr obj) {
		registered_objects_it_ = find(registered_objects_.begin(),
			registered_objects_.end(), obj);

		if (registered_objects_it_ == registered_objects_.end())
			registered_objects_.push_back(obj);
	}

	void TeteManager::operator-=(ObjectPtr obj) {
		registered_objects_it_ = find(registered_objects_.begin(),
			registered_objects_.end(), obj);

		if (registered_objects_it_ == registered_objects_.end())
			return;

		registered_objects_.erase(registered_objects_it_);
	}
}
