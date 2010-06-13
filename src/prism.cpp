
#include <vector>

#include "prism.h"

namespace Atelier {
    Prism::Prism() {
        for(int i = 0; i < 8; ++i)
            points_.push_back(Vec3D());
        position_ = Vec3D();
    }

    Prism::Prism(const std::vector<Vec3D>& in_points) {
        points_ = in_points;
    }

    Prism::Prism(const Vec3D& center, float x_dim, float y_dim, float z_dim) {
        float hx = x_dim / 2.0f;
        float hy = y_dim / 2.0f;
        float hz = z_dim / 2.0f;
        position_ = center;
        points_.push_back(Vec3D(center.x + hx, center.y + hy, center.z + hz));
        points_.push_back(Vec3D(center.x + hx, center.y + hy, center.z - hz));
        points_.push_back(Vec3D(center.x + hx, center.y - hy, center.z + hz));
        points_.push_back(Vec3D(center.x + hx, center.y - hy, center.z - hz));
        points_.push_back(Vec3D(center.x - hx, center.y + hy, center.z + hz));
        points_.push_back(Vec3D(center.x - hx, center.y + hy, center.z - hz));
        points_.push_back(Vec3D(center.x - hx, center.y - hy, center.z + hz));
        points_.push_back(Vec3D(center.x - hx, center.y - hy, center.z - hz));
    }

    Prism::Prism(const Vec3D& pt1, const Vec3D& pt2, const Vec3D& pt3, const Vec3D& pt4,
            const Vec3D& pt5, const Vec3D& pt6, const Vec3D& pt7, const Vec3D& pt8) {
        points_.push_back(pt1);
        points_.push_back(pt2);
        points_.push_back(pt3);
        points_.push_back(pt4);
        points_.push_back(pt5);
        points_.push_back(pt6);
        points_.push_back(pt7);
        points_.push_back(pt8);
    }

    void Prism::set_points(const std::vector<Vec3D>& new_points) {
        if(points_.empty() == false)
            points_.clear();

        points_ = new_points;
    }

    Vec3D& Prism::position() {
        return position_;
    }

    void Prism::set_position(const Vec3D& new_pos) {
        position_ = new_pos;
    }
}