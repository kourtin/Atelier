
#include "cinderInteractItem.h"
#include "basicApp.h"
#include "client.h"

namespace Atelier {
    CinderInteractItem::CinderInteractItem() {
        Client::app().addListener(this);

        detect_selection_ = false;
    }

    CinderInteractItem::~CinderInteractItem() {
        Client::app().removeListener(this);
    }

    bool CinderInteractItem::resize(int, int) {
        return false;
    }

    bool CinderInteractItem::mouseDown(ci::app::MouseEvent mouse_event) {
        if(detect_selection_)
            detect_click_selection(mouse_event);

        return false;
    }

    bool CinderInteractItem::mouseUp(ci::app::MouseEvent) {
        return false;
    }

    bool CinderInteractItem::mouseDrag(ci::app::MouseEvent) {
        return false;
    }

    bool CinderInteractItem::mouseWheel(ci::app::MouseEvent) {
        return false;
    }
}