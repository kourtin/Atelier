
#pragma once

#include "objectInterface.h"
#include "identity.h"

namespace Atelier {

    class Object : public virtual ObjectInterface {
    public:
        Object(ID new_id);
        virtual ~Object();

        const ID& id();
        const ID& owner();
        Identity identity();

        virtual Vec3D position();
        virtual Vec3D rotation();
        virtual Vec3D scale();

        virtual void set_position(Vec3D);
        virtual void set_rotation(Vec3D);
        virtual void set_scale(Vec3D);

    private:
        ID id_;
        ID owner_;
        Vec3D position_;
        Vec3D rotation_;
        Vec3D scale_;
    };
}