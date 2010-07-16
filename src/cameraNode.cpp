
#include "cinder/gl/gl.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Camera.h"

#include <define.h>
#include <cameraNode.h>
#include <identity.h>
#include <genericNode.h> // Needed for testing
#include <client.h>
#include <basicApp.h>
#include <tete.h>
#include <gridsNetworkItem.h>
#include <link.h>
#include <userNode.h>

namespace Atelier {
    CameraNode::CameraNode(const ID& new_id) : Object(new_id) {
		//ci::app::console() << "Creating CameraNode" << std::endl;

        cam_ = new ci::MayaCamUI();
        zoom_speed_ = 30.0f;
        far_clip_ = 10000.0f;

        create_camera();
    }

    const ID& CameraNode::id() {
        return Object::id();
    }

    std::list<const Link*>& CameraNode::links() {
        return Object::links();
    }

	const std::list<const Link*>& CameraNode::links() const {
        return Object::links();
    }

    Vec3D CameraNode::position() const {
        return Object::position();
    }

    Vec3D CameraNode::rotation() const {
        return Object::rotation();
    }

    Vec3D CameraNode::scale() const {
        return Object::scale();
    }

    void CameraNode::set_position(Vec3D vec) {
        Object::set_position(vec);
    }

    void CameraNode::set_rotation(Vec3D vec) {
        Object::set_rotation(vec);
    }
    
    void CameraNode::set_scale(Vec3D vec) {
        Object::set_scale(vec);
    }

    void CameraNode::call_matrix() {
        ci::gl::setMatrices(cam_->getCamera());
    }

    Rect CameraNode::bounding_rect() const {
        return Rect();
    }

    Prism CameraNode::bounding_prism() const {
        return Prism();
    }

    AABox CameraNode::bounding_aabox() const {
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

    void CameraNode::create_object(const Tete&) {
    }
    
    void CameraNode::update_object(const Tete&) {
    }

    void CameraNode::update_object_matrix(const Tete&) {
    }

    void CameraNode::request_create_object(const Value&) {
    }

    void CameraNode::request_update_object(const Value&) {
    }

	void CameraNode::request_create(UserNodePtr node) {
		Tete request;
		
        const Identity* ident = node->identity();
        LinkPtr ident_link(new Link(ident, LinkFlags(true, true, true)));
        request.links().push_back(ident_link);
		request.attr()["type"] = "CameraNode";

		GridsNetworkItem::request_create_object(request);
	}
}