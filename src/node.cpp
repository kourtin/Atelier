
#include "node.h"
#include "basicApp.h"

namespace Atelier {
    Node::Node(const ID& new_id) : Object(new_id) {
    }

    Node::~Node() {
    }

    const ID& Node::id() {
        return Object::id();
    }

    Vec3D Node::position() const {
        return Object::position();
    }

    Vec3D Node::rotation() const {
        return Object::rotation();
    }

    Vec3D Node::scale() const {
        return Object::scale();
    }
}