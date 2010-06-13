
#pragma once

#include <vector>

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "define.h"

namespace Atelier {
    class GraphicItem;
    class CameraNode;
    class StaticGraphicItem;

    class Renderer {
    public:
        Renderer();

        void init();

        void set_camera(CameraNode*);
        void add_item(GraphicItem*);
        void add_item(GraphicItem&);
        void remove_item(GraphicItem*);

        void update(); // Should be called after the camera node is updated 
        void render();

        void add_static_item(StaticGraphicItem*);
        void add_static_item(StaticGraphicItem&);

        void call_camera_matrix();

        const Vec3D& billboard_right() const;
        const Vec3D& billboard_up() const;

        const CameraNode& camera();

    private:
        void begin3d();
        void end3d();
        void draw3d();
        void begin2d();
        void end2d();
        void draw2d();

        void draw_items(RenderDimension, GLenum);

        std::vector<GraphicItem*> items_;
        std::vector<StaticGraphicItem*> static_items_;
        CameraNode* camera_;
        Vec3D billboard_right_;
        Vec3D billboard_up_;
    };
}
