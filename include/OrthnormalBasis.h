//
// Created by Dustin on 9/9/26.
//

#ifndef WHITTED_ORTHNORMALBASIS_H
#define WHITTED_ORTHNORMALBASIS_H

#include "Vector.h"

struct OrthonormalBasis {
    OrthonormalBasis(Vec3D f, Vec3D u);
    OrthonormalBasis(Vec3D e1, Vec3D e2, Vec3D e3);

    Vec3D e1_;
    Vec3D e2_;
    Vec3D e3_;
};


#endif //WHITTED_ORTHNORMALBASIS_H
