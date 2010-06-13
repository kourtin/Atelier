
#pragma once

#include "graphicItem.h"
#include "define.h"
#include "prism.h"

namespace Atelier {
    class CinderGraphicItem : public GraphicItem {
    public:
        virtual ~CinderGraphicItem();

        virtual Prism bounding_prism();
        virtual Rect bounding_rect();
        virtual void render(RenderDimension, RenderPass);
    };
}