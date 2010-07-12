
#include <objectCreator.h>
#include <tete.h>
#include <object.h>
#include <genericNode.h>
#include <cameraNode.h>
#include <client.h>
#include <userNode.h>

namespace Atelier {
	ObjectCreator::ObjectCreator() {
		instance_ = this;
	}

	ObjectCreator* ObjectCreator::instance_;

	ObjectCreator& ObjectCreator::instance() {
		return *instance_;
	}

	void ObjectCreator::receive_tete(const Tete& tete) {
		if (tete.type() == Tete::CREATE)
			create_object(tete);
		else if (tete.type() == Tete::UPDATE) 
			update_object(tete);
	}

	Object* ObjectCreator::create_object(const Tete& tete) {
		std::string type = tete.value()["attr"]["type"].asString();

		Object* created_object = NULL;

		if (type == "CameraNode")
			created_object = new CameraNode(tete.id());
		else if (type == "GenericNode")
			created_object = new GenericNode(tete.id());
		else if (type == "UserNode")
			created_object = new UserNode(tete.id());
		else
			created_object = new GenericNode(tete.id());

		if (created_object == NULL) {
			ci::app::console() << "WARNING: could not find object type" << std::endl;
			return NULL;
		}

		created_object->create_object(tete);

		CinderGraphicItem* graphic_item = 
			dynamic_cast<CinderGraphicItem*>(created_object);

		if (graphic_item != NULL)
            Client::renderer() += graphic_item;

		return created_object;
	}

	Object* ObjectCreator::update_object(const Tete& tete) {
		return NULL;
	}
}