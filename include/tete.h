
#pragma once

#include <vector>
#include <queue>
#include <list>

#include "define.h"

namespace Atelier {
	class Link;
	class Identity;

	typedef std::list<const Link*> LinkList;

    class Tete {
    public:
        Tete();
        Tete(const LinkList& links, const Value&);
		~Tete();

        enum Type {
            INVALID,
            CREATE_ROOM,
            LIST_ROOMS,
            CREATE,
            UPDATE,
            UPDATE_VALUE,
            UPDATE_MATRIX
        };

        bool has_links() const;
        const LinkList& links() const;
		LinkList links();
        const Value& value() const;
		Value& value();
        Type type() const;

        void set_type_from_value(const Value&);

		const ID& id() const;
		bool has_matrix() const;
		Vec3D position() const;
		Vec3D rotation() const;
		Vec3D scale() const;
		const Value& attr() const;
		Value& attr();
		void set_position(Vec3D);
		void set_rotation(Vec3D);
		void set_scale(Vec3D);

    private:
        Value value_;
        LinkList links_;
        Type type_;
		ID id_;

	/////////////////////////////
	// Static utility methods
	////////////////////////////
	public:
		static Tete* create_tete(const LinkList&, 
			const Value&);
		static Tete* next_tete();
		static bool queued_tetes();
		
		static bool linked_to_identity(const Identity&, const Tete&);
		// Returns creator, or NULL if there is none
		static const Identity* get_creator(const Tete&);
    };
}