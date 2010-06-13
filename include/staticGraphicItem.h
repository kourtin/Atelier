
#pragma once

/* Objects displayed in 3D space or 2D that organize or beautify 
   the space. */

#include "define.h"

namespace Atelier {
    class StaticGraphicItem { 
    public:
        virtual ~StaticGraphicItem() { };

        virtual void render(RenderDimension, RenderPass) = 0;
    };
}