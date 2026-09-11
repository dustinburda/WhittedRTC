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
    CameraInterface(Point3D origin, Point3D look_at, Vec3D up);

    virtual ~CameraInterface() = default;

    const Point3D& Origin() const;
    void SetSampler(std::shared_ptr<Sampler> s);

    virtual Ray GetRayAt(int x, int y, const ImagePlane& p) const = 0;

protected:
    std::shared_ptr<Sampler> s_;

    Point3D origin_;
    Transformation t_;
};

#endif //CAMERAINTERFACE_H
