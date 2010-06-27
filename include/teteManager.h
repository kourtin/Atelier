
/* TeteManager
Handles the queing and delivery of Tetes
*/

#pragma once

#include <queue>

namespace Atelier {
	class Tete;

	class TeteManager {
	public:
		TeteManager();

		static TeteManager& instance();

		void update();

		friend class Tete;

	private:
		static TeteManager* tete_manager_;

		std::queue<Tete*> tete_queue_;
	};
}
