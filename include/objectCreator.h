
/* ObjectCreator
	Basically this object uses the type information from a Tete
	and generates an object.
	By far the most jenk piece of code...
*/

#pragma once

namespace Atelier {
	class Object;
	class Tete;
	class GenericNode;
	class CameraNode;

	class ObjectCreator {
	public:
		ObjectCreator();

		static ObjectCreator& instance();

		Object* create_object(const Tete&); // cannot be const Tete& for unknown MSVS error

	private:
		static ObjectCreator* instance_;

		GenericNode* create_generic_node(const Tete&);
		CameraNode* create_camera_node(const Tete&);
	};
}