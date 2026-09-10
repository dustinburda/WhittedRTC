//
// Created by Dustin on 12/1/24.
//

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Ray.h"
#include "Matrix.h"
#include "Normal.h"
#include "Point.h"
#include "OrthnormalBasis.h"
#include "Vector.h"

class Transformation {
public:
    // Orthonormal basis constructor

    static Transformation WorldToLocal(Point3D origin, OrthonormalBasis basis);

    static Transformation Identity();

    static Transformation Translation(double x, double y, double z);

    static Transformation RotationX(double radians);
    static Transformation RotationY(double radians);
    static Transformation RotationZ(double radians);

    static Transformation ReflectionX();
    static Transformation ReflectionY();
    static Transformation ReflectionZ();

    static Transformation Scale(double x, double y, double z);

    // pbrt
    Point<double, 3> operator()(const Point<double, 3>& p) const;
    Vector<double, 3> operator()(const Vector<double, 3>& v) const;
    Normal<double, 3> operator()(const Normal<double, 3>& n) const;
    Ray operator()(const Ray& r) const;

    // pbrt
    Point<double, 3> ApplyInverse(const Point<double, 3>& p) const;
    Vector<double, 3> ApplyInverse(const Vector<double, 3>& v) const;
    Normal<double, 3> ApplyInverse(const Normal<double, 3>& n) const;
    Ray ApplyInverse(const Ray& r) const;

    Transformation operator*(const Transformation& other) {
        Transformation t;
        t.transform_ = transform_ * other.transform_;
        t.inverse_transform_ = other.inverse_transform_ * inverse_transform_;

        return t;
    }

private:
    Transformation() = default;

    Matrix<double, 4, 4> transform_;
    Matrix<double, 4, 4> inverse_transform_;
};


#endif //TRANSFORMATION_H
