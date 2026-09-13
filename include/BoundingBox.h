//
// Created by Dustin on 12/22/24.
//

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Point.h"
#include "Ray.h"
#include "ShadeContext.h"

#include <algorithm>

struct Interval {
    Interval() = default;
    explicit Interval(double t_min, double t_max) : t_min_ {std::min(t_min, t_max)}, t_max_ {std::max(t_min, t_max)} {}

    bool operator<(const Interval& other) const;
    static bool Intersects(std::vector<Interval> intervals);

    double t_min_;
    double t_max_;
};

// Not templated for now
class BoundingBox {
public:
    BoundingBox() : min_{0.0 ,0.0 ,0.0}, max_{1.0, 1.0, 1.0} {}
    BoundingBox(Point3D p1, Point3D p2);

    bool Hit(const Ray& r, ShadeContext& context) const;

    Point3D Min() const { return min_; }
    Point3D Max() const { return max_; }

    double Width();
    double Height();
    double Length();

    std::string toString() const;


private:
    Point3D min_;
    Point3D max_;
};

BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2);



#endif //BOUNDINGBOX_H
