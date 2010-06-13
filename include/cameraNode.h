
#pragma once

namespace cinder {
    class MayaCamUI;
}

#include "basicApp.h"
#include "define.h"
#include "object.h"
#include "cinder3DInteractItem.h"
#include "networkItem.h"
#include "cinderGraphicItem.h"
#include "prism.h"

namespace Atelier {
    class CameraNode : public Object, public Cinder3DInteractItem,
        public CinderGraphicItem, public NetworkItem {
    public:
        CameraNode(ID);

        void call_matrix();

        virtual Rect bounding_rect();
        virtual Prism bounding_prism();

        virtual void receive_tete(const Tete&);

        bool mouseDown(ci::app::MouseEvent);
        bool mouseDrag(ci::app::MouseEvent);
        bool mouseWheel(ci::app::MouseEvent);

        ci::MayaCamUI* cam() { return cam_; }
    private:
        ci::MayaCamUI* cam_;
        float zoom_speed_;
        float far_clip_;
    };
}