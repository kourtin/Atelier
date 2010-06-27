
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

    const ID& Node::owner() {
        return Object::owner();
    }

    Vec3D Node::position() {
        return Object::position();
    }

    Vec3D Node::rotation() {
        return Object::rotation();
    }

    Vec3D Node::scale() {
        return Object::scale();
    }
}