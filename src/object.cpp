
#include <object.h>
#include <objectController.h>
#include <identity.h>

namespace Atelier {

    Object::Object(const ID& new_id) {
        id_ = new_id;

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
}