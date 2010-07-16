
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

    void Node::set_position(Vec3D vec) {
        Object::set_position(vec);
    }

    void Node::set_rotation(Vec3D vec) {
        Object::set_rotation(vec);
    }
    
    void Node::set_scale(Vec3D vec) {
        Object::set_scale(vec);
    }

    void Node::activate(const Identity& ident) {
        GridsNetworkItem::activate(ident);
    }

    std::list<const Link*>& Node::links() {
        return Object::links();
    }

	const std::list<const Link*>& Node::links() const {
        return Object::links();
    }

    void Node::update_object(const Tete& tete) {
        Object::update_object(tete);
    }

    void Node::create_object(const Tete& tete) {
        Object::create_object(tete);
    }

}