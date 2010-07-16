
/* TeteManager
Handles the queing and delivery of Tetes
*/

#pragma once

#include <queue>
#include <list>

namespace Atelier {
	class Tete;
	class Object;

    typedef std::tr1::shared_ptr<Object> ObjectPtr;

	class TeteManager {
	public:
		TeteManager();

		static TeteManager& instance();

		void update();

		void operator+=(ObjectPtr obj); // register object for Tete delivery
		void operator-=(ObjectPtr obj); // unregister object

		friend class Tete;

	private:
        void distribute_update(const Tete&);

		static TeteManager* tete_manager_;

		std::queue<Tete*> tete_queue_;
		std::list<ObjectPtr> registered_objects_;
		std::list<ObjectPtr>::const_iterator registered_objects_it_;
	};
}
