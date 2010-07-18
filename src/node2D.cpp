
#include <node2D.h>
#include <define.h>

namespace Atelier {
	Node2D::Node2D(const ID& new_id) : Object(new_id) {
		container_ = NULL;
	}

	Node2D::~Node2D() {
	}

	const ID& Node2D::id() const {
		return Object::id();
	}

	std::list<LinkConstPtr>& Node2D::links() {
		return Object::links();
	}

	const std::list<LinkConstPtr>& Node2D::links() const {
		return Object::links();
	}

	Vec3D Node2D::position() const {
		return Object::position();
	}

	Vec3D Node2D::rotation() const {
		return Object::rotation();
	}

	Vec3D Node2D::scale() const {
		return Object::scale();
	}

    void Node2D::set_position(Vec3D vec) {
        Object::set_position(vec);
    }

    void Node2D::set_rotation(Vec3D vec) {
        Object::set_rotation(vec);
    }
    
    void Node2D::set_scale(Vec3D vec) {
        Object::set_scale(vec);
    }
    
	void Node2D::create_object(const Tete& tete) {
        Object::create_object(tete);
	}

	void Node2D::update_object(const Tete& tete) {
        Object::update_object(tete);
	}

	const InteractItem& Node2D::container() {
		return *container_;
	}

	void Node2D::set_container(InteractItem& item) {
		container_ = &item;
	}

    void Node2D::activate(const Identity& ident) {
        GridsNetworkItem::activate(ident);
    }
}