
#include <tete.h>
#include <identity.h>
#include <grids/interface.h>
#include <grids/protocol.h>
#include <utility.h>
#include <teteManager.h>

namespace Atelier {
    Tete::Tete() {
        type_ = INVALID;
		value_["id"] = Utility::create_uuid();
    }

    Tete::Tete(const std::vector<const Identity*>& link_idents, const Value& val) {
        links_ = link_idents;
        value_ = val;
        // Parse value_ to figure out Type
        set_type_from_value(val);
    }

    Tete::Type Tete::type() const {
        return type_;
    }

    bool Tete::has_links() const {
        return !links_.empty();
    }

    const std::vector<const Identity*>& Tete::links() const {
        return links_;
    }

	std::vector<const Identity*> Tete::links() {
		return links_;
	}

    const Value& Tete::value() const {
        return value_;
    }

	Value& Tete::value() {
		return value_;
	}

    void Tete::set_type_from_value(const Value& val) {
        if (val["_method"].empty()) {
            type_ = INVALID;
            return;
        }

        std::string method = val["_method"].asString();

        if (method == Grids::Interface::GRIDS_CREATE_ROOM)
            type_ = CREATE_ROOM;
        else if (method == Grids::Interface::GRIDS_LIST_ROOMS)
            type_ = LIST_ROOMS;
        else if (method == Grids::Interface::GRIDS_CREATE_OBJECT)
            type_ = CREATE;
        else if (method == Grids::Interface::GRIDS_UPDATE_OBJECT)
            type_ = UPDATE;
        else
            type_ = INVALID;
    }

	
	const ID& Tete::id() const {
		return value_["id"].asString();
	}

	bool Tete::has_matrix() const {
		return !value_["pos"].empty() && !value_["rot"].empty()
			&& !value_["scl"].empty();
	}

	Vec3D Tete::position() const {
		return Vec3D(value_["pos"][0u].asDouble(), value_["pos"][1u].asDouble(),
			value_["pos"][2u].asDouble());
	}

	Vec3D Tete::rotation() const {
		return Vec3D(value_["rot"][0u].asDouble(), value_["rot"][1u].asDouble(),
			value_["rot"][2u].asDouble());
	}

	Vec3D Tete::scale() const {
		return Vec3D(value_["scl"][0u].asDouble(), value_["scl"][1u].asDouble(),
			value_["scl"][2u].asDouble());
	}

	const Value& Tete::attr() const {
		return value_["attr"];
	}

	Value& Tete::attr() {
		return value_["attr"];
	}

	void Tete::set_position(Vec3D pos) {
		value_["pos"][0u] = pos.x;
		value_["pos"][1u] = pos.y;
		value_["pos"][2u] = pos.z;
	}

	void Tete::set_rotation(Vec3D rot) {
		value_["rot"][0u] = rot.x;
		value_["rot"][1u] = rot.y;
		value_["rot"][2u] = rot.z;
	}

	void Tete::set_scale(Vec3D scl) {
		value_["scl"][0u] = scl.x;
		value_["scl"][1u] = scl.y;
		value_["scl"][2u] = scl.z;
	}

	////////////////////////
	// Static
	///////////////////////

	Tete* Tete::create_tete(const std::vector<const Identity*>& idents, const Value& val) {
		Tete* tete = new Tete(idents, val);

		TeteManager::instance().tete_queue_.push(tete);

		return tete;
	}

	bool Tete::queued_tetes() {
		return !TeteManager::instance().tete_queue_.empty();
	}

	Tete* Tete::next_tete() {
		if (TeteManager::instance().tete_queue_.empty())
			return NULL;

		Tete* tete = TeteManager::instance().tete_queue_.front();
		TeteManager::instance().tete_queue_.pop();
		return tete;
	}

}