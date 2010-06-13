
// Technically this class should be called Hexahedron, 
// but that seemed a bit longwinded...

#pragma once

#include <vector>

#include "define.h"

namespace Atelier {
    class Prism {
    public:
        Prism();
        Prism(const std::vector<Vec3D>&);
        Prism(const Vec3D&, float, float, float); // center, x-dim, y-dim, z-dim
        Prism(const Vec3D&, const Vec3D&, const Vec3D&, const Vec3D&,
            const Vec3D&, const Vec3D&, const Vec3D&, const Vec3D&);

        const std::vector<Vec3D>& points();
        void set_points(const std::vector<Vec3D>&);

        Vec3D& position();
        void set_position(const Vec3D&);

    private:
        std::vector<Vec3D> points_;
        Vec3D position_;
    };
}