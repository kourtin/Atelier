
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
	class UserNode;

    class CameraNode : public Object, public Cinder3DInteractItem,
        public CinderGraphicItem, public GridsNetworkItem {
    public:
        CameraNode(const ID&);

        void call_matrix();

        virtual Rect bounding_rect() const;
        virtual Prism bounding_prism() const;
        virtual AABox bounding_aabox() const;

        virtual void receive_tete(const Tete&);
        virtual void activate(const Identity&);

        bool resize(int, int);
        bool mouseDown(ci::app::MouseEvent);
        bool mouseDrag(ci::app::MouseEvent);
        bool mouseWheel(ci::app::MouseEvent);

        ci::MayaCamUI* cam() const { return cam_; }

        void create_camera();

		virtual std::string type() { return "CameraNode"; }

		static void request_create(UserNode*);

        virtual void request_create_object(const Value&);
        virtual void request_update_object(const Value&);

        virtual void create_object(const Tete&);
        virtual void update_object(const Tete&);
        virtual void update_object_matrix(const Tete&);

    private:
        ci::MayaCamUI* cam_;
        float zoom_speed_;
        float far_clip_;
    };
}