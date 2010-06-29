
#pragma once

#include "define.h"
#include "prism.h"
#include "aaBox.h"

namespace Atelier {
    class InteractItemInterface {
    public:
        virtual Rect bounding_rect() const = 0;
        virtual Prism bounding_prism() const = 0;
        virtual AABox bounding_aabox() const = 0;
    };
}