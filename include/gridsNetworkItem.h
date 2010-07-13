
#pragma once

#include "networkItem.h"

namespace Atelier {
    class GridsNetworkItem : public NetworkItem {
    public:
        virtual ~GridsNetworkItem();

        virtual void activate(const Identity&);

        virtual void request_create_room(const Value&);

		static void request_create_object(Tete&);
        static void request_update_object(Tete&);

        virtual void request_create_object(Value&);
        virtual void request_create_object(const Vec3D&,
            const Vec3D&, const Vec3D&, Value&);
        virtual void request_update_object(Value&);

        virtual void request_update_position(const Vec3D&);
        virtual void request_update_rotation(const Vec3D&);
        virtual void request_update_scale(const Vec3D&);
        virtual void request_update_object_matrix(const Vec3D&,
            const Vec3D&, const Vec3D&);

    protected:
        virtual void request_update_object_internal(Value&);
        virtual void request_create_object_internal(Value&);
    };
}