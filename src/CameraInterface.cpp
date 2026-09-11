#include "../include/CameraInterface.h"

CameraInterface::CameraInterface(Point3D origin, Point3D look_at, Vec3D up)
    : origin_{origin}, t_ { Transformation::WorldToLocal(origin_, OrthonormalBasis{look_at - origin, up}) }
{
    auto forward = (look_at - origin);

    if ((look_at - origin).LengthSquared() < epsilon)
        throw std::logic_error ("Camera look_at point must differ from origin.");

    if (Cross(forward, up).LengthSquared()  < epsilon)
        throw std::logic_error ("Camera forward vector and up vector must not be parallel.");
}

const Point3D& CameraInterface::Origin() const { return origin_; }

void CameraInterface::SetSampler(std::shared_ptr<Sampler> s) { s_ = s; }