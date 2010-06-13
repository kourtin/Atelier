
#pragma once

#include "define.h"

namespace Atelier {

    class ObjectInterface {
    public:
        virtual const ID& id() = 0;

        virtual const ID& owner() = 0;

        virtual Vec3D position() = 0;
        virtual Vec3D rotation() = 0;
        virtual Vec3D scale() = 0;

        virtual void set_position(Vec3D) = 0;
        virtual void set_rotation(Vec3D) = 0;
        virtual void set_scale(Vec3D) = 0;

        virtual void receive_tete(const Tete&) = 0;
    };
}