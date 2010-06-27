
#include <objectCreator.h>
#include <tete.h>
#include <object.h>
#include <genericNode.h>
#include <cameraNode.h>

namespace Atelier {
	ObjectCreator::ObjectCreator() {
		instance_ = this;
	}

	ObjectCreator* ObjectCreator::instance_;

	ObjectCreator& ObjectCreator::instance() {
		return *instance_;
	}

	Object* ObjectCreator::create_object(const Tete& tete) {
		std::string type = tete.value()["attr"]["type"].asString();

		if (type == "CameraNode")
			return create_camera_node(tete);
		else if (type == "GenericNode")
			return create_generic_node(tete);
		else
			return create_generic_node(tete);
	}

	GenericNode* ObjectCreator::create_generic_node(const Tete& tete) {
		GenericNode* node = new GenericNode(tete.id());
		node->create_object(tete);

		return node;
	}

	CameraNode* ObjectCreator::create_camera_node(const Tete& tete) {
		return NULL;
	}
}