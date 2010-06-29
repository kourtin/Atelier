
#pragma once

namespace Atelier {
    class Tete;

    class NetworkItemInterface {
    public:
        virtual void receive_tete_from_network(Tete&) = 0;

        virtual void request_create_room(const Value&) = 0;
        //virtual void request_create_object(const Value&) = 0;
        //virtual void request_create_object(const Vec3D&,
        //    const Vec3D&, const Vec3D&, Value&) = 0;
        //virtual void request_update_object(const Value&) = 0;
        //virtual void request_update_position(const Vec3D&) = 0;
        //virtual void request_update_rotation(const Vec3D&) = 0;
        //virtual void request_update_scale(const Vec3D&) = 0;
        //virtual void request_update_object_matrix(const Vec3D&,
        //    const Vec3D&, const Vec3D&) = 0;
    };
}