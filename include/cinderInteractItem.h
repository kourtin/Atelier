
#pragma once

#include "cinder/app/App.h"

#include "interactItem.h"

namespace Atelier {
    class CinderInteractItem : public InteractItem,
        public ci::app::App::Listener {
    public:
        CinderInteractItem();
        virtual ~CinderInteractItem();

        virtual void detect_click_selection(ci::app::MouseEvent) = 0;

        // If a method returns true, event propegation stops. If it returns false,
        // the message propegates to other Listeners.
        virtual bool resize(int, int);
        virtual bool mouseDown(ci::app::MouseEvent);
        virtual bool mouseUp(ci::app::MouseEvent);
        virtual bool mouseDrag(ci::app::MouseEvent);
        virtual bool mouseWheel(ci::app::MouseEvent);
    };
}