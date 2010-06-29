
#pragma once

#include "cinderInteractItem.h"

namespace Atelier {
    class Cinder2DInteractItem : public CinderInteractItem {
    public:
        Cinder2DInteractItem();
        virtual ~Cinder2DInteractItem();

		// All 2D items are inside of an object in 3D space
		virtual const InteractItem& container() const = 0;

		Prism bounding_prism() const;
		AABox bounding_aabox() const;

        void detect_click_selection(ci::app::MouseEvent);
    };
}