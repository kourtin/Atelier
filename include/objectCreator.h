
/* ObjectCreator
	Basically this object uses the type information from a Tete
	and generates an object.
	Of any single piece of code, this needs work.
*/

#pragma once

#include <object.h>

namespace Atelier {
	class Object;
	class Tete;
	class GenericNode;
	class CameraNode;

	class ObjectCreator {
	public:
		ObjectCreator();

		static ObjectCreator& instance();

		// Note: these methods should not be considered time-critical
		void receive_tete(const Tete&);
		ObjectPtr create_object(const Tete&);
		void update_object(const Tete&);

	private:
		static ObjectCreator* instance_;
	};
}