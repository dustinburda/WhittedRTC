//
// Created by Dustin on 12/22/24.
//

#include "../include/BoundingBox.h"


bool Interval::operator<(const Interval& other) const {
    if (t_min_ < other.t_min_)
        return true;
    if (t_min_ > other.t_min_)
        return false;
    return t_max_ < other.t_max_;
}

bool Interval::Intersects(std::vector<Interval> intervals) {
    if (intervals.size() == 0)
        return true;

    std::sort(intervals.begin(), intervals.end());

    Interval intersection = intervals[0];
    for(std::size_t i = 1; i < intervals.size(); i++) {
        const Interval& curr_interval = intervals[i];
        if(curr_interval.t_min_ > intersection.t_max_)
            return false;

        intersection.t_min_ = std::max(intersection.t_min_, curr_interval.t_min_);
        intersection.t_max_ = std::min(intersection.t_max_, curr_interval.t_max_);

    }

    return true;
}

BoundingBox::BoundingBox(Point3D p1, Point3D p2) {
    for(int i = 0; i < 3; i++) {
        min_[i] = std::min(p1[i], p2[i]);
        max_[i] = std::max(p1[i], p2[i]);
    }
}

bool BoundingBox::Hit(const Ray& r, [[ maybe_unused ]] ShadeContext& context) const {
    Point3D orig = r.Origin();
    Vec3D dir = r.Direction();

    Interval x_interval {(min_[0] - orig[0])/dir[0], (max_[0] - orig[0])/dir[0] };
    Interval y_interval {(min_[1] - orig[1])/dir[1], (max_[1] - orig[1])/dir[1] };
    Interval z_interval {(min_[2] - orig[2])/dir[2], (max_[2] - orig[2])/dir[2] };

    if (context.t_max_ < std::max({x_interval.t_min_, y_interval.t_min_, z_interval.t_min_}) ||
        context.t_min_ > std::min({x_interval.t_max_, y_interval.t_max_, z_interval.t_max_})) {
        return false;
    }

    return Interval::Intersects({x_interval, y_interval, z_interval});
}

double BoundingBox::Width() {
    return std::abs(max_[0] - min_[0]);
}

double BoundingBox::Height() {
    return std::abs(max_[1] - min_[1]);
}

double BoundingBox::Length() {
    return std::abs(max_[2] - min_[2]);
}

std::string BoundingBox::toString() const {
    std::stringstream ss;

    ss << min_.toString() << "\n" << max_.toString();

    return ss.str();
}

BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2) {
    Point3D min1 = b1.Min();
    Point3D max1 = b1.Max();

    Point3D min2 = b2.Min();
    Point3D max2 = b2.Max();

    Point3D new_min;
    Point3D new_max;

    for(int i = 0; i < 3; i++) {
        new_min[i] = std::min(min1[i], min2[i]);
        new_max[i] = std::max(max1[i], max2[i]);
    }

    return {new_min, new_max};
}

