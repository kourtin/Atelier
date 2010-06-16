
#pragma once

#include "cinder/AxisAlignedBox.h"

#include "define.h"

namespace Atelier {
    class AABox : public ci::AxisAlignedBox3f {
    public:
        AABox();
        AABox(const Vec3D& min_vec, const Vec3D& max_vex);

        void glDraw();
    };
}