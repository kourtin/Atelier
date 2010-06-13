
#pragma once

#include "define.h"
#include "prism.h"

namespace Atelier {
    class GraphicItemInterface {
    public:
        virtual Rect bounding_rect() = 0;
        virtual Prism bounding_prism() = 0;

        virtual void render(RenderDimension, RenderPass) = 0;
    };
}