//
// Created by Dustin on 12/6/24.
//

#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "CameraInterface.h"

class OrthographicCamera : public CameraInterface {
public:
    OrthographicCamera() = delete;

    OrthographicCamera(Point3D origin, Point3D look_at_, Vec3D up, double h_fov)
        : CameraInterface(origin, look_at_, up, h_fov) {}

    virtual ~OrthographicCamera() = default;

    Ray GetRayAt(int x, int y, const ImagePlane& p) const override;

private:

};



#endif //ORTHOGRAPHICCAMERA_H
