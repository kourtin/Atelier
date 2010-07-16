
#include <node2D.h>
#include <define.h>

namespace Atelier {
	Node2D::Node2D(const ID& new_id) : Object(new_id) {
		container_ = NULL;
	}

	Node2D::~Node2D() {
	}

	const ID& Node2D::id() {
		return Object::id();
	}

	std::list<const Link*>& Node2D::links() {
		return Object::links();
	}

	const std::list<const Link*>& Node2D::links() const {
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

	const InteractItem& Node2D::container() {
		return *container_;
	}

	void Node2D::set_container(InteractItem& item) {
		container_ = &item;
	}
}