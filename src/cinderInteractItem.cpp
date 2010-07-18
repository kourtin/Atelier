
#include "cinderInteractItem.h"
#include "basicApp.h"
#include "client.h"

namespace Atelier {
    CinderInteractItem::CinderInteractItem() {
        Client::app().addListener(this);

        detect_selection_ = false;
        draggable_ = false;
        drag_in_progress_ = false;
    }

    CinderInteractItem::~CinderInteractItem() {
        Client::app().removeListener(this);
    }

    bool CinderInteractItem::resize(int, int) {
        return false;
    }

    bool CinderInteractItem::mouseDown(ci::app::MouseEvent mouse_event) {
        if(detect_selection_) {
            bool select = detect_click_selection(mouse_event);

            if (draggable_ && select) {
                drag_in_progress_ = true;
                drag_start(mouse_event);
            }
        }

        return false;
    }

    bool CinderInteractItem::mouseUp(ci::app::MouseEvent mouse_event) {
        if (drag_in_progress_) {
            drag_end(mouse_event);
            drag_in_progress_ = false;
        }

        return false;
    }

    bool CinderInteractItem::mouseDrag(ci::app::MouseEvent mouse_event) {
        if (drag_in_progress_) {
            dragging(mouse_event);
        }

        return false;
    }

    bool CinderInteractItem::mouseWheel(ci::app::MouseEvent) {
        return false;
    }

    void CinderInteractItem::drag_start(ci::app::MouseEvent) {
    }
    
    void CinderInteractItem::dragging(ci::app::MouseEvent) {
    }
    
    void CinderInteractItem::drag_end(ci::app::MouseEvent) {
    }
}