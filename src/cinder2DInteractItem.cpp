
#include "cinder2DInteractItem.h"

namespace Atelier {
    Cinder2DInteractItem::Cinder2DInteractItem() {
    }

    Cinder2DInteractItem::~Cinder2DInteractItem() {
    }

    void Cinder2DInteractItem::detect_click_selection(ci::app::MouseEvent event) {
        // Use the bounding rect to detect if the object has been pressed
    }

	Prism Cinder2DInteractItem::bounding_prism() const {
		return container().bounding_prism();
	}

	AABox Cinder2DInteractItem::bounding_aabox() const {
		return container().bounding_aabox();
	}

}