//
// Created by Dustin on 12/6/24.
//

#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include "ImagePlane.h"
#include "Ray.h"
#include "Sampler.h"

#include <memory>

#include "Transformation.h"

class CameraInterface {
public:
    CameraInterface() = delete;

    CameraInterface(Point3D origin, Point3D look_at, Vec3D up, double h_fov)
        : origin_{origin}, h_fov_{h_fov}, t_ { Transformation::WorldToLocal(origin_, OrthonormalBasis{look_at - origin, up}) }
    {
        auto forward = (look_at - origin);

        if ((look_at - origin).LengthSquared() < epsilon)
            throw std::logic_error ("Camera look_at point must differ from origin.");

        if (Dot(forward, up) < epsilon)
            throw std::logic_error ("Camera forward vector and up vector must not be parallel.");
    }

    virtual ~CameraInterface() = default;


    const Point3D& Origin() const { return origin_; }
    virtual Ray GetRayAt(int x, int y, const ImagePlane& p) const = 0;
    void SetSampler(std::shared_ptr<Sampler> s) { s_ = s; }

protected:
    std::shared_ptr<Sampler> s_;

    Point3D origin_;
    double h_fov_;

    Transformation t_;
};

#endif //CAMERAINTERFACE_H
