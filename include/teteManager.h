
/* TeteManager
Handles the queing and delivery of Tetes
*/

#pragma once

namespace Atelier {
	class TeteManager {
	public:
		TeteManager();

		static TeteManager& instance();

		void update();

	private:
		static TeteManager* tete_manager_;
	};
}
