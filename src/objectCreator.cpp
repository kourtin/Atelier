
#include <objectCreator.h>
#include <tete.h>
#include <object.h>
#include <genericNode.h>
#include <cameraNode.h>
#include <client.h>
#include <userNode.h>
#include <chatNode.h>
#include <chatMessageNode.h>
#include <identity.h>

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

	ObjectPtr ObjectCreator::create_object(const Tete& tete) {
		std::string type = tete.value()["attr"]["type"].asString();

		//Object* created_object = NULL;
        ObjectPtr created_object;

		if (type == "CameraNode")
			created_object = ObjectPtr(new CameraNode(tete.id()));
		else if (type == "GenericNode")
			created_object = ObjectPtr(new GenericNode(tete.id()));
		else if (type == "UserNode")
			created_object = ObjectPtr(new UserNode(tete.id()));
        else if (type == "ChatNode")
            created_object = ObjectPtr(new ChatNode(tete.id()));
        else if (type == "ChatMessageNode")
            created_object = ObjectPtr(new ChatMessageNode(tete.id()));
		else
			created_object = ObjectPtr(new GenericNode(tete.id()));

        if (created_object.get() == NULL) {
			ci::app::console() << "WARNING: could not find object type" << std::endl;
			return NULL;
		}

		std::tr1::shared_ptr<GraphicItem> graphic_item = 
			std::dynamic_pointer_cast<GraphicItem>(created_object);

		if (graphic_item.get() != NULL)
            Client::renderer() += graphic_item;

        ObjectController::instance() += created_object;
        Identity::create_identity(tete.id(), created_object);

        created_object->create_object(tete);

		return created_object;
	}

	void ObjectCreator::update_object(const Tete& tete) {
		// Nothing here
	}
}