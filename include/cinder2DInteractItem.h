
#pragma once

#include "cinderInteractItem.h"

namespace Atelier {
    class Cinder2DInteractItem : public CinderInteractItem {
    public:
        Cinder2DInteractItem();
        virtual ~Cinder2DInteractItem();

        void detect_click_selection(ci::app::MouseEvent);
    };
}