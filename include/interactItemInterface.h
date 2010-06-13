
#pragma once

#include "define.h"
#include "prism.h"

namespace Atelier {
    class InteractItemInterface {
    public:
        virtual Rect bounding_rect() = 0;
        virtual Prism bounding_prism() = 0;
    };
}