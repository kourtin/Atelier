
#include "cinder/Ray.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Cinder.h"

#include "cinder3DInteractItem.h"
#include "client.h"
#include "cameraNode.h"
#include "basicApp.h"

namespace Atelier {
    Cinder3DInteractItem::Cinder3DInteractItem() {
    }

    Cinder3DInteractItem::~Cinder3DInteractItem() {
    }

    void Cinder3DInteractItem::detect_click_selection(ci::app::MouseEvent mouse_event) {
        if(!detect_selection_)
            return;
        float cam_x = static_cast<float>(mouse_event.getX()) / 
            static_cast<float>(Client::app().getWindowWidth());
        float cam_y = static_cast<float>(mouse_event.getY()) / 
            static_cast<float>(Client::app().getWindowHeight());

        cam_y = 1.0f - cam_y; // TODO: why do I need to do this? Is this a bug?
        
        // NOTE: make sure the camera has the proper aspect ratio set
        ci::Ray ray = Client::active_camera().cam()->getCamera().generateRay(cam_x, cam_y, Client::app().getWindowAspectRatio());

        float box_times[2];
        // See if the ray intersects the bounding box
        if(bounding_aabox().intersect(ray, box_times) > 0) {
            activate(Client::user_identity());
        }

        //set_position(ray.getOrigin() + ray.getDirection()* 1000.0f);
    }
}