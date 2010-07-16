
#pragma once

#include "graphicItem.h"
#include "define.h"
#include "prism.h"
#include "aaBox.h"

namespace Atelier {
    class CinderGraphicItem : public GraphicItem {
    public:
        virtual ~CinderGraphicItem();

        virtual Prism bounding_prism() const;
        virtual Rect bounding_rect() const;
        virtual AABox bounding_aabox() const;
        virtual void render(RenderDimension, RenderPass);
    };

    typedef std::tr1::shared_ptr<CinderGraphicItem> CinderGraphicItemPtr;
}