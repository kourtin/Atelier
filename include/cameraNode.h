
#pragma once

namespace cinder {
    class MayaCamUI;
}

#include "define.h"
#include "object.h"
#include "cinder3DInteractItem.h"
#include <gridsNetworkItem.h>
#include "cinderGraphicItem.h"
#include "prism.h"
#include "aaBox.h"

namespace Atelier {
    class Tete;
    class Identity;

    class CameraNode : public Object, public Cinder3DInteractItem,
        public CinderGraphicItem, public GridsNetworkItem {
    public:
        CameraNode(ID);

        void call_matrix();

        virtual Rect bounding_rect();
        virtual Prism bounding_prism();
        virtual AABox bounding_aabox();

        virtual void receive_tete(const Tete&);
        virtual void activate(const Identity&);

        bool resize(int, int);
        bool mouseDown(ci::app::MouseEvent);
        bool mouseDrag(ci::app::MouseEvent);
        bool mouseWheel(ci::app::MouseEvent);

        ci::MayaCamUI* cam() const { return cam_; }

        virtual void create_object(const Value&);
        virtual void update_object(const Value&);
        virtual void update_object_matrix(const Value&);

    private:
        void create_camera();

        ci::MayaCamUI* cam_;
        float zoom_speed_;
        float far_clip_;
    };
}