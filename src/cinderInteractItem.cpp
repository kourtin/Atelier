
#include "cinderInteractItem.h"
#include "basicApp.h"
#include "client.h"

namespace Atelier {
    CinderInteractItem::CinderInteractItem() {
        Client::app().addListener(this);
    }

    CinderInteractItem::~CinderInteractItem() {
        Client::app().removeListener(this);
    }

    bool CinderInteractItem::mouseDown(ci::app::MouseEvent) {
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