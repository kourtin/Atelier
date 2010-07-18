
#include <cinder/app/MouseEvent.h>

#include <cinder2DInteractItem.h>
#include <client.h>

namespace Atelier {
    Cinder2DInteractItem::Cinder2DInteractItem() {
    }

    Cinder2DInteractItem::~Cinder2DInteractItem() {
    }

    bool Cinder2DInteractItem::detect_click_selection(ci::app::MouseEvent mouse_event) {
        if (!detect_selection_)
            return false;

        Rect bounds = bounding_rect();
        bounds *= scale().x;
        bounds += ci::Vec2f(position().x, position().y);

        if (bounds.isInside(ci::Vec2f(mouse_event.getX(), mouse_event.getY()))) {
            activate(Client::user_identity());
            return true;
        }

        return false;
    }

	Prism Cinder2DInteractItem::bounding_prism() const {
		return container().bounding_prism();
	}

	AABox Cinder2DInteractItem::bounding_aabox() const {
		return container().bounding_aabox();
	}
}