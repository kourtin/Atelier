
#include <object.h>
#include <objectController.h>
#include <identity.h>
#include <tete.h>

namespace Atelier {

    Object::Object(const ID& new_id) {
        id_ = new_id;

        position_ = Vec3D(0.0, 0.0, 0.0);
        rotation_ = Vec3D(0.0, 0.0, 0.0);
        scale_ = Vec3D(1.0, 1.0, 1.0);

		ObjectController::instance() += this;
        Identity::create_identity(id_, this); // registers identity
    }

    Object::~Object() {
		ObjectController::instance() -= this;
    }

    const ID& Object::id() {
        return id_;
    }

	std::list<const Identity*>& Object::links() {
		return links_;
	}

    const std::list<const Identity*>& Object::links() const {
		return links_;
    }

    Vec3D Object::position() const {
        return position_;
    }

    Vec3D Object::rotation() const {
        return rotation_;
    }

    Vec3D Object::scale() const {
        return scale_;
    }

    void Object::set_position(Vec3D vec) {
        position_ = vec;
    }

    void Object::set_rotation(Vec3D vec) {
        rotation_ = vec;
    }

    void Object::set_scale(Vec3D vec) {
        scale_ = vec;
    }
    /*
    Identity* Object::identity() {
        Identity::get_identity_or_create(this);
    }
    */

    const Identity* Object::identity() const {
        return Identity::get_identity_from_id(id_);
    }

    void Object::create_object(const Tete& tete) {
        if (!tete.value()["pos"].empty()) {
            position_ = Vec3D(tete.value()["pos"][0u].asDouble(),
                tete.value()["pos"][1u].asDouble(), 
                tete.value()["pos"][2u].asDouble());
        }

        if (!tete.value()["rot"].empty()) {
            rotation_ = Vec3D(tete.value()["rot"][0u].asDouble(),
                tete.value()["rot"][1u].asDouble(),
                tete.value()["rot"][2u].asDouble());
        }

        if (!tete.value()["scl"].empty()) {
            scale_ = Vec3D(tete.value()["scl"][0u].asDouble(),
                tete.value()["scl"][1u].asDouble(),
                tete.value()["scl"][2u].asDouble());
        }
        // TODO: finish this...
    }
}