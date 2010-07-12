
#pragma once

#include "objectInterface.h"
#include "identity.h"

namespace Atelier {

    class Object : public virtual ObjectInterface {
    public:
        Object(const ID& object_id);
        virtual ~Object();

        const ID& id();
		std::list<const Identity*>& links();
		const std::list<const Identity*>& links() const;
        //Identity* identity();
        const Identity* identity() const;

        virtual Vec3D position() const;
        virtual Vec3D rotation() const;
        virtual Vec3D scale() const;

        virtual void set_position(Vec3D);
        virtual void set_rotation(Vec3D);
        virtual void set_scale(Vec3D);

        virtual void create_object(const Tete&);

    private:
        ID id_;
        std::list<const Identity*> links_;
        Vec3D position_;
        Vec3D rotation_;
        Vec3D scale_;
    };
}