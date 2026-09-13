//
// Created by Dustin on 12/22/24.
//

#include "../include/BVH.h"

#include "../include/Util.h"

BVHNode::BVHNode(Instance instance)
    : left_{nullptr}, right_{nullptr}, bounding_box_{instance.BBox()}, instance_ {instance}, is_leaf_{true} {}

BVHNode::BVHNode(std::unique_ptr<BVHNode> left, std::unique_ptr<BVHNode> right)
    : left_ {std::move(left)}, right_ {std::move(right)}, bounding_box_{Union(left_->bounding_box_, right_->bounding_box_)}, instance_{std::nullopt}, is_leaf_{false} {}

bool BVHNode::Hit(const Ray& r, ShadeContext& context) {
    if (!bounding_box_.Hit(r, context))
        return false;

    if (is_leaf_) {
        return instance_->Hit(r, context);
    }

    bool left = left_->Hit(r, context);
    bool right = right_->Hit(r, context);

    return left || right;
}

BVH::BVH() : root_{nullptr} {}

BVH::BVH(std::vector<Instance>& shapes) {
    root_ = Build(shapes);
}

bool BVH::Hit(const Ray& r, ShadeContext& s) {
    if (root_ == nullptr)
        return false;

    return root_->Hit(r, s);
}

std::unique_ptr<BVHNode> BVH::Build(std::vector<Instance> shapes) {
    if (shapes.size() == 0)
        return nullptr;

    return BuildRecursive(shapes);
}

std::unique_ptr<BVHNode> BVH::BuildRecursive(std::span<Instance> shapes) const {
    if (shapes.size() == 1) {
        return std::make_unique<BVHNode>(shapes[0]);
    }

    int split_axis = RandomInt(0, 2);

    // TODO: replace basic comparator with Surface Area Heuristic
    auto comparator = [&split_axis](Instance i1, Instance i2) {
        return i1.BBox().Min()[split_axis] < i2.BBox().Min()[split_axis];
    };

    std::sort(shapes.begin(), shapes.end(), comparator);

    std::span<Instance> s1 {shapes.begin(), shapes.begin() + shapes.size() / 2};
    std::span<Instance> s2 {shapes.begin() + shapes.size() / 2, shapes.end()};

    auto left = BuildRecursive(s1);
    auto right = BuildRecursive(s2);

    return std::make_unique<BVHNode>(std::move(left), std::move(right));
}