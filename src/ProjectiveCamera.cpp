//
// Created by Advil on 11/3/2024.
//

#include "../include/ProjectiveCamera.h"
#include "../include/Util.h"

Ray ProjectiveCamera::GetRayAt(int x, int y, const ImagePlane& p) const
{
    auto aspect_ratio = static_cast<double>(p.width_) / p.height_;

    auto ray_plane_width = 2.0 * tan(h_fov_ / 2.0);
    auto ray_plane_height = ray_plane_width / aspect_ratio;

    auto v_x = Vec3D {ray_plane_width, 0.0, 0.0} * (1.0 / p.width_);
    auto v_y = Vec3D {0.0, -ray_plane_height, 0.0} * (1.0 / p.height_);

    auto upper_left_corner = Point3D {
        -ray_plane_width / 2.0,
        ray_plane_height / 2.0,
        1.0
    };

    auto point = upper_left_corner + (x + 0.5) * v_x + (y + 0.5) * v_y;

    Point3D local_origin {0.0, 0.0 , 0.0};

    auto local_ray = Ray{
        local_origin ,
        point - local_origin ,
        RayType::PrimaryRay
    };

    return t_.ApplyInverse(local_ray);
}