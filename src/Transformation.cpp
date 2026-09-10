//
// Created by Dustin on 12/1/24.
//

#include "../include/Transformation.h"


Transformation Transformation::WorldToLocal( [[ maybe_unused ]] Point3D origin, [[ maybe_unused ]] OrthonormalBasis basis) {
    Transformation t;

    // TODO: Fill out transformation matrix

    t.inverse_transform_ = Inverse(t.transform_);
    return t;
}

Transformation Transformation::Identity() {
    Transformation t;
    t.transform_ = {1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0};

    t.inverse_transform_ = Inverse(t.transform_);
    return t;
}

Transformation Transformation::Translation(double x, double y, double z) {
    Transformation t;
    t.transform_ = {1.0, 0.0, 0.0, x,
                      0.0, 1.0, 0.0, y,
                      0.0, 0.0, 1.0, z,
                      0.0, 0.0, 0.0, 1.0};
    t.inverse_transform_ = Inverse(t.transform_);
    return t;

}

Transformation Transformation::RotationX(double radians) {
    Transformation t;
    double costheta = std::cos(radians);
    double sintheta = std::sin(radians);
    t.transform_ = {1.0, 0.0,      0.0,       0.0,
                      0.0, costheta, -sintheta, 0.0,
                      0.0, sintheta, costheta,  0.0,
                      0.0, 0.0,      0.0,       1.0};
    t.inverse_transform_ = Inverse(t.transform_);
    return t;
}

Transformation Transformation::RotationY(double radians) {
    Transformation t;
    double costheta = std::cos(radians);
    double sintheta = std::sin(radians);
    t.transform_ = {costheta,  0.0,  sintheta,  0.0,
                      0.0,       1.0,  0.0,       0.0,
                      -sintheta, 0.0,  costheta,  0.0,
                      0.0,       0.0,  0.0,       1.0};
    t.inverse_transform_ = Inverse(t.transform_);
    return t;
}

Transformation Transformation::RotationZ(double radians) {
    Transformation t;
    double costheta = std::cos(radians);
    double sintheta = std::sin(radians);
    t.transform_ = {costheta, -sintheta, 0.0, 0.0,
                      sintheta, costheta,  0.0, 0.0,
                      0.0,      0.0,       1.0, 0.0,
                      0.0,      0.0,       0.0, 1.0};
    t.inverse_transform_ = Inverse(t.transform_);

    return t;
}

Transformation Transformation::ReflectionX() {
    Transformation t;
    t.transform_ = {-1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0};
    t.inverse_transform_ = Inverse(t.transform_);

    return t;
}

Transformation Transformation::ReflectionY() {
    Transformation t;
    t.transform_ = {1.0, 0.0, 0.0, 0.0,
                      0.0, -1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0};
    t.inverse_transform_ = Inverse(t.transform_);

    return t;
}

Transformation Transformation::ReflectionZ() {
    Transformation t;
    t.transform_ = {1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, -1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0};
    t.inverse_transform_ = Inverse(t.transform_);

    return t;
}

Transformation Transformation::Scale(double x, double y, double z) {
    Transformation t;
    t.transform_ = {x,   0.0, 0.0, 0.0,
                      0.0, y,   0.0, 0.0,
                      0.0, 0.0, z,   0.0,
                      0.0, 0.0, 0.0, 1.0};
    t.inverse_transform_ = Inverse(t.transform_);

    return t;
}

Point<double, 3> Transformation::operator()(const Point<double, 3>& p) const {
    auto homogenous_coordiante = transform_[3][0] * p[0] + transform_[3][1] * p[1] + transform_[3][2] * p[2] + transform_[3][3];
    return Point<double, 3> {(transform_[0][0] * p[0] + transform_[0][1] * p[1] + transform_[0][2] * p[2] + transform_[0][3]) / homogenous_coordiante,
                             (transform_[1][0] * p[0] + transform_[1][1] * p[1] + transform_[1][2] * p[2] + transform_[1][3]) / homogenous_coordiante,
                             (transform_[2][0] * p[0] + transform_[2][1] * p[1] + transform_[2][2] * p[2] + transform_[2][3]) / homogenous_coordiante};
}
Vector<double, 3> Transformation::operator()(const Vector<double, 3>& v) const {
    return Vector<double, 3> {(transform_[0][0] * v[0] + transform_[0][1] * v[1] + transform_[0][2] * v[2]),
                              (transform_[1][0] * v[0] + transform_[1][1] * v[1] + transform_[1][2] * v[2]),
                              (transform_[2][0] * v[0] + transform_[2][1] * v[1] + transform_[2][2] * v[2])};
}

Normal<double, 3> Transformation::operator()(const Normal<double, 3>& n) const {
    return Normal<double, 3> {(inverse_transform_[0][0] * n[0] + inverse_transform_[1][0] * n[1] + inverse_transform_[2][0] * n[2]),
                              (inverse_transform_[0][1] * n[0] + inverse_transform_[1][1] * n[1] + inverse_transform_[2][1] * n[2]),
                              (inverse_transform_[0][2] * n[0] + inverse_transform_[1][2] * n[1] + inverse_transform_[2][2] * n[2])};
}
Ray Transformation::operator()(const Ray& r) const {
    return Ray{(*this)(r.Origin()), (*this)(r.Direction()), RayType::PrimaryRay};
}

Point<double, 3> Transformation::ApplyInverse(const Point<double, 3>& p) const {
    auto homogenous_coordiante = inverse_transform_[3][0] * p[0] + inverse_transform_[3][1] * p[1] + inverse_transform_[3][2] * p[2] + inverse_transform_[3][3];
    return Point<double, 3> {(inverse_transform_[0][0] * p[0] + inverse_transform_[0][1] * p[1] + inverse_transform_[0][2] * p[2] + inverse_transform_[0][3]) / homogenous_coordiante,
                             (inverse_transform_[1][0] * p[0] + inverse_transform_[1][1] * p[1] + inverse_transform_[1][2] * p[2] + inverse_transform_[1][3]) / homogenous_coordiante,
                             (inverse_transform_[2][0] * p[0] + inverse_transform_[2][1] * p[1] + inverse_transform_[2][2] * p[2] + inverse_transform_[2][3]) / homogenous_coordiante};
}
Vector<double, 3> Transformation::ApplyInverse(const Vector<double, 3>& v) const {
    return Vector<double, 3> {(inverse_transform_[0][0] * v[0] + inverse_transform_[0][1] * v[1] + inverse_transform_[0][2] * v[2]),
                          (inverse_transform_[1][0] * v[0] + inverse_transform_[1][1] * v[1] + inverse_transform_[1][2] * v[2]),
                          (inverse_transform_[2][0] * v[0] + inverse_transform_[2][1] * v[1] + inverse_transform_[2][2] * v[2])};
}
Normal<double, 3> Transformation::ApplyInverse(const Normal<double, 3>& n) const {
    return Normal<double, 3> {(transform_[0][0] * n[0] + transform_[1][0] * n[1] + transform_[2][0] * n[2]),
                          (transform_[0][1] * n[0] + transform_[1][1] * n[1] + transform_[2][1] * n[2]),
                          (transform_[0][2] * n[0] + transform_[1][2] * n[1] + transform_[2][2] * n[2])};
}
Ray Transformation::ApplyInverse(const Ray& r) const {
    return Ray{this->ApplyInverse(r.Origin()), this->ApplyInverse(r.Direction()), RayType::PrimaryRay};
}