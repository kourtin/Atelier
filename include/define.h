
#pragma once

#include <string>

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"

namespace Atelier {
    typedef std::string ID;
    typedef std::string Tete;

    typedef ci::Vec3f Vec3D;
    typedef ci::Rectf Rect;
    typedef GLenum RenderPass;

    enum RenderDimension {
        TWO_D,
        THREE_D
    };
}