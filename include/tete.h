
#pragma once

#include <vector>
#include <queue>

#include "define.h"
#include "identity.h"

namespace Atelier {
    class Tete {
    public:
        Tete();
        Tete(const std::vector<const Identity*>& links, const Value&);

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
        const std::vector<const Identity*>& links() const;
		std::vector<const Identity*> links();
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
        std::vector<const Identity*> links_;
        Type type_;

	/////////////////////////////
	// Static Utility Stuff
	////////////////////////////
	public:
		static Tete* create_tete(const std::vector<const Identity*>&, 
			const Value&);
		static Tete* next_tete();
		static bool queued_tetes();

	private:
		static std::queue<Tete*> tete_queue_;
    };
}