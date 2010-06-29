
#pragma once

#include "define.h"
#include "graphicItemInterface.h"
#include "objectInterface.h"

namespace Atelier {
    class GraphicItem : public GraphicItemInterface,
                        public virtual ObjectInterface {
    public:
        virtual ~GraphicItem();

        //virtual Rect bounding_rect() const;
        //virtual void render(RenderDimension, RenderPass);
    };
}