
#include "cinder/gl/gl.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Camera.h"

#include "define.h"
#include "cameraNode.h"
#include "identity.h"
#include "genericNode.h" // Needed for testing
#include "client.h"
#include "basicApp.h"
#include "tete.h"
#include <gridsNetworkItem.h>

namespace Atelier {
    CameraNode::CameraNode(ID new_id) : Object(new_id) {
        cam_ = new ci::MayaCamUI();
        zoom_speed_ = 30.0f;
        far_clip_ = 10000.0f;

        create_camera();
    }

    void CameraNode::call_matrix() {
        ci::gl::setMatrices(cam_->getCamera());
    }

    Rect CameraNode::bounding_rect() {
        return Rect();
    }

    Prism CameraNode::bounding_prism() {
        return Prism();
    }

    AABox CameraNode::bounding_aabox() {
        return CinderGraphicItem::bounding_aabox();
    }

    void CameraNode::receive_tete(const Tete&) {
    }

    bool CameraNode::resize(int, int) {
        create_camera();
        return false;
    }

    bool CameraNode::mouseDown(ci::app::MouseEvent mouse_event) {
        cam_->mouseDown(mouse_event.getPos());

        return false;
    }

    bool CameraNode::mouseDrag(ci::app::MouseEvent event) {
        cam_->mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), 
            event.isRightDown());
        return false;
    }

    // Zoom code taken from Cinder::MayaCamUI
    bool CameraNode::mouseWheel(ci::app::MouseEvent event) {
        float mouse_delta = event.getWheelIncrement() * zoom_speed_;
        //float direction = mouse_delta > 0.0f ? 1.0 : -1.0;
        
        float newCOI = powf( 2.71828183f, -mouse_delta / 500.0f ) * cam_->getCamera().getCenterOfInterest();
        Vec3D oldTarget = cam_->getCamera().getCenterOfInterestPoint();
        Vec3D newEye = oldTarget - cam_->getCamera().getViewDirection() * newCOI;
        ci::CameraPersp new_cam = cam_->getCamera();
        new_cam.setEyePoint( newEye );
        new_cam.setCenterOfInterest( newCOI );
        cam_->setCurrentCam(new_cam);

        return false;
    }

    void CameraNode::activate(const Identity& ident) {

    }

    void CameraNode::create_camera() {
        ci::CameraPersp new_cam = cam_->getCamera();
        new_cam.setFarClip(far_clip_);
        new_cam.setAspectRatio(Client::app().getWindowAspectRatio());
        cam_->setCurrentCam(new_cam);
    }

    void CameraNode::create_object(const Value&) {
    }
    
    void CameraNode::update_object(const Value&) {
    }

    void CameraNode::update_object_matrix(const Value&) {
    }
}