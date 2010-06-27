
#pragma once

#include <string>

#include <json/json.h>

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"

namespace Atelier {
    // NOTE: if the ID changes type, overloaded operators in 
    // Atelierr::Identity will need to be updated.
    typedef std::string ID;
    typedef std::string GridsID;
    typedef Json::Value Value;

    typedef ci::Vec3f Vec3D;
    typedef ci::Rectf Rect;
    typedef GLenum RenderPass;

    enum RenderDimension {
        TWO_D,
        THREE_D
    };

    static char* INVALID_ID = "invalid";
}