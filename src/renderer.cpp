
#include "cinder/MayaCamUI.h"

#include "renderer.h"
#include "graphicItem.h"
#include "cameraNode.h"
#include "staticGraphicItem.h"
#include "client.h"
#include "cameraNode.h"
#include "basicApp.h"

namespace Atelier {
    Renderer::Renderer() {
        camera_ = NULL;
    }

    void Renderer::init() {
    }

    void Renderer::set_camera(CameraNode* cam) {
        camera_ = cam;
    }

    void Renderer::operator+=(GraphicItem& item) {
        add_item(item);
    }

    void Renderer::operator+=(GraphicItem* item) {
        add_item(item);
    }

    void Renderer::operator-=(GraphicItem& item) {
        remove_item(&item);
    }

    void Renderer::operator-=(GraphicItem* item) {
        remove_item(item);
    }

    void Renderer::add_item(GraphicItem* item) {
        if(item == NULL)
            return;

        items_.push_back(item);
    }

    void Renderer::add_item(GraphicItem& item) {
        add_item(&item);
    }

    void Renderer::remove_item(GraphicItem* item) {
        // TODO: write this
    }

    void Renderer::render() {
        begin3d();
        draw3d();
        end3d();

        //begin2d();
        //draw2d();
        //end2d();
    }

    void Renderer::begin3d() {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
        ci::gl::enableAlphaBlending();
        ci::gl::enableDepthRead(true);
        ci::gl::enableDepthWrite(true);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void Renderer::end3d() {
    }

    void Renderer::draw3d() {
        draw_items(THREE_D, GL_LINES);
    }

    void Renderer::draw_items(RenderDimension dim, GLenum pass) {
        for (std::vector<GraphicItem*>::const_iterator it = items_.begin(); it != items_.end(); ++it) {
            (*it)->render(dim, pass);
        }

        for (std::vector<StaticGraphicItem*>::const_iterator it = static_items_.begin(); it != static_items_.end(); ++it) {
            (*it)->render(dim, pass);
        }
    }

    void Renderer::begin2d() {
        ci::gl::enableDepthWrite( false );
        ci::gl::setMatricesWindow( Client::app().getWindowSize() );
    }

    void Renderer::draw2d() {
        //glBegin(GL_LINES);
        draw_items(RenderDimension::TWO_D, GL_LINES);
        //glEnd();
    }

    void Renderer::end2d() {
    }

    void Renderer::add_static_item(StaticGraphicItem& item) {
        add_static_item(&item);
    }

    void Renderer::add_static_item(StaticGraphicItem* item) {
        if(item == NULL)
            return;

        static_items_.push_back(item);
    }

    void Renderer::call_camera_matrix() {
        if(camera_)
            camera_->call_matrix();
    }

    void Renderer::update() {
		if (camera_ == NULL)
			return;

        camera_->cam()->getCamera().getBillboardVectors(&billboard_right_, &billboard_up_);
    }

    const Vec3D& Renderer::billboard_right() const {
        return billboard_right_;
    }

    const Vec3D& Renderer::billboard_up() const {
        return billboard_up_;
    }

    const CameraNode& Renderer::camera() {
        return *camera_;
    }
}