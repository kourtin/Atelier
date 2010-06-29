
/* TeteManager
Handles the queing and delivery of Tetes
*/

#pragma once

#include <queue>
#include <list>

namespace Atelier {
	class Tete;
	class Object;

	class TeteManager {
	public:
		TeteManager();

		static TeteManager& instance();

		void update();

		void operator+=(Object* obj); // register object for Tete delivery
		void operator-=(Object* obj); // unregister object

		friend class Tete;

	private:
		static TeteManager* tete_manager_;

		std::queue<Tete*> tete_queue_;
		std::list<Object*> registered_objects_;
		std::list<Object*>::const_iterator registered_objects_it_;
	};
}
