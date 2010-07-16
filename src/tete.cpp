
#include <cinder/app/App.h>

#include <tete.h>
#include <identity.h>
#include <grids/interface.h>
#include <grids/protocol.h>
#include <utility.h>
#include <teteManager.h>
#include <link.h>

namespace Atelier {
    Tete::Tete() {
        type_ = INVALID;
		value_["id"] = Utility::create_uuid();
		id_ = value_["id"].asString();
    }

    Tete::Tete(const LinkList& links_, const Value& val) {
        this->links_ = links_;
        value_ = val;
        // Parse value_ to figure out Type
        set_type_from_value(val);
		id_ = value_["id"].asString();
    }

	Tete::~Tete() {
	}

    Tete::Type Tete::type() const {
        return type_;
    }

    bool Tete::has_links() const {
        return !links_.empty();
    }

    const LinkList& Tete::links() const {
        return links_;
    }

	LinkList& Tete::links() {
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
		return id_;
	}

	bool Tete::has_matrix() const {
		return !value_["pos"].empty() && !value_["rot"].empty()
			&& !value_["scl"].empty();
	}

	Vec3D Tete::position() const {
		return Vec3D(
            static_cast<float>(value_["pos"][0u].asDouble()), 
            static_cast<float>(value_["pos"][1u].asDouble()),
			static_cast<float>(value_["pos"][2u].asDouble()));
	}

	Vec3D Tete::rotation() const {
		return Vec3D(
            static_cast<float>(value_["rot"][0u].asDouble()), 
            static_cast<float>(value_["rot"][1u].asDouble()),
			static_cast<float>(value_["rot"][2u].asDouble()));
	}

	Vec3D Tete::scale() const {
		return Vec3D(
            static_cast<float>(value_["scl"][0u].asDouble()), 
            static_cast<float>(value_["scl"][1u].asDouble()),
			static_cast<float>(value_["scl"][2u].asDouble()));
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

	Tete* Tete::create_tete(const LinkList& links, const Value& val) {
		Tete* tete = new Tete(links, val);

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

	bool Tete::linked_to_identity(const Identity& ident, const Tete& tete) {
		const LinkList& links = tete.links();

		for(LinkList::const_iterator it = links.begin(); it != links.end(); ++it) {
			if (it->get()->actor() == ident)
				return true;
		}
		return false;
	}

	const Identity* Tete::get_creator(const Tete& tete) {
		if (tete.has_links() == false) {
            ci::app::console() << "WARNING: Cannot find creator" << std::endl
                << "Offending Tete:" << std::endl << tete.value().toStyledString();

			return NULL;
        }
        
		const LinkList& links = tete.links();

		for(LinkList::const_iterator it = links.begin(); it != links.end(); ++it) {
			if (it->get()->flags().creator) {
				return &((*it)->actor());
            }
		}

        ci::app::console() << "WARNING: Cannot find creator" << std::endl
            << "Offending Tete:" << std::endl << tete.value().toStyledString();

		return NULL;
	}
}
