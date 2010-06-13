
#pragma once

#include "cinderInteractItem.h"

namespace Atelier {
    class Cinder3DInteractItem : public CinderInteractItem {
    public:
        Cinder3DInteractItem();
        virtual ~Cinder3DInteractItem();

        void detect_click_selection(ci::app::MouseEvent);
    };
}