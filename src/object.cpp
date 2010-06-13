
#include "object.h"

namespace Atelier {

    Object::Object(ID new_id) {
        id_ = new_id;
    }

    Object::~Object() {
    }

    const ID& Object::id() {
        return id_;
    }

    const ID& Object::owner() {
        return owner_;
    }

    Vec3D Object::position() {
        return position_;
    }

    Vec3D Object::rotation() {
        return rotation_;
    }

    Vec3D Object::scale() {
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
}