
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

        virtual bool mouseDown(ci::app::MouseEvent);
        virtual bool mouseUp(ci::app::MouseEvent);
        virtual bool mouseDrag(ci::app::MouseEvent);
        virtual bool mouseWheel(ci::app::MouseEvent);
    };
}