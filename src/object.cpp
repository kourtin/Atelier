
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <object.h>
#include <objectController.h>
#include <identity.h>
#include <tete.h>

namespace Atelier {

    Object::Object(const ID& new_id) {
        id_ = new_id;

        position_ = Vec3D(0.0f, 0.0f, 0.0f);
        rotation_ = Vec3D(0.0f, 0.0f, 0.0f);
        scale_ = Vec3D(1.0f, 1.0f, 1.0f);

		//ObjectController::instance() += this;
        //Identity::create_identity(id_, this); // registers identity
    }

    Object::~Object() {
		//ObjectController::instance() -= this;
    }

    const ID& Object::id() {
        return id_;
    }

	std::list<const Link*>& Object::links() {
		return links_;
	}

    const std::list<const Link*>& Object::links() const {
		return links_;
    }

    Vec3D Object::position() const {
        ScopedLock l(position_mutex_);
        return position_;
    }

    Vec3D Object::rotation() const {
        ScopedLock l(rotation_mutex_);
        return rotation_;
    }

    Vec3D Object::scale() const {
        ScopedLock l(scale_mutex_);
        return scale_;
    }

    void Object::set_position(Vec3D vec) {
        ScopedLock l(position_mutex_);
        position_ = vec;
    }

    void Object::set_rotation(Vec3D vec) {
        ScopedLock l(rotation_mutex_);
        rotation_ = vec;
    }

    void Object::set_scale(Vec3D vec) {
        ScopedLock l(scale_mutex_);
        scale_ = vec;
    }

    void Object::lock_position() const {
        position_mutex_.lock();
    }

    void Object::lock_rotation() const {
        rotation_mutex_.lock();
    }

    void Object::lock_scale() const {
        scale_mutex_.lock();
    }

    void Object::unlock_position() const {
        position_mutex_.unlock();
    }

    void Object::unlock_rotation() const {
        rotation_mutex_.unlock();
    }

    void Object::unlock_scale() const {
        scale_mutex_.unlock();
    }

    void Object::lock_links() const {
        links_mutex_.lock();
    }

    void Object::unlock_links() const {
        links_mutex_.unlock();
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
        set_matrix_from_tete(tete);
        // TODO: finish this...
    }

    void Object::update_object(const Tete& tete) {
        set_matrix_from_tete(tete);
    }

    void Object::set_matrix_from_tete(const Tete& tete) {
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
    }
}