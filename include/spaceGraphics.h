
#pragma once

#include "staticGraphicItem.h"

namespace Atelier {
    class SpaceGraphics : public StaticGraphicItem {
    public:
        SpaceGraphics();
        virtual ~SpaceGraphics();

        void render(RenderDimension, RenderPass);

    private:
        void draw_axis();
        void draw_grid_plane_lines();
        void init_line_weights();

        float grid_plane_size_;
        int grid_plane_intervals_;
    };
}