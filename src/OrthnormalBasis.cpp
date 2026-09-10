//
// Created by Dustin on 9/9/26.
//

#include "../include/OrthnormalBasis.h"


OrthonormalBasis::OrthonormalBasis(Vec3D f, Vec3D u) {
    auto forward = f.UnitVector();
    auto right = Cross(u.UnitVector(), forward).UnitVector();
    auto ortho_up = Cross(forward, right).UnitVector();

    e1_ = right;
    e2_ = ortho_up;
    e3_ = forward;
}

OrthonormalBasis::OrthonormalBasis(Vec3D e1, Vec3D e2, Vec3D e3) : e1_{e1.UnitVector()}, e2_{e2.UnitVector()}, e3_{e3.UnitVector()} {}