
#include "cinderGraphicItem.h"
#include "prism.h"

namespace Atelier {
    CinderGraphicItem::~CinderGraphicItem() {
    }

    Rect CinderGraphicItem::bounding_rect() const {
        return Rect();
    }

    Prism CinderGraphicItem::bounding_prism() const {
        return Prism();
    }

    AABox CinderGraphicItem::bounding_aabox() const {
        return AABox(position() + scale(), position() - scale());
    }

    void CinderGraphicItem::render(RenderDimension dim, RenderPass pass) {
    }
}