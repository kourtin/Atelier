
#include "cinderGraphicItem.h"
#include "prism.h"

namespace Atelier {
    CinderGraphicItem::~CinderGraphicItem() {
    }

    Rect CinderGraphicItem::bounding_rect() {
        return Rect();
    }

    Prism CinderGraphicItem::bounding_prism() {
        return Prism();
    }

    void CinderGraphicItem::render(RenderDimension dim, RenderPass pass) {
    }
}