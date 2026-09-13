//
// Created by Dustin on 12/22/24.
//

#ifndef BVH_H
#define BVH_H

#include "BoundingBox.h"
#include "Instance.h"

#include <span>
#include <memory>
#include <optional>

enum class SplitMethod {
    RandomAxis,
    SurfaceAreaHeuristic,

};

struct BVHNode {
public:
    explicit BVHNode(Instance instance);
    BVHNode(std::unique_ptr<BVHNode> left, std::unique_ptr<BVHNode> right);

    bool Hit(const Ray& r, ShadeContext& context);

private:
    std::unique_ptr<BVHNode> left_;
    std::unique_ptr<BVHNode> right_;
    BoundingBox bounding_box_;

    std::optional<Instance> instance_;
    bool is_leaf_;

};

class BVH {
public:
    BVH();
    explicit BVH(std::vector<Instance>& shapes);
    bool Hit(const Ray& r, ShadeContext& s);
    std::unique_ptr<BVHNode> Build(std::vector<Instance> shapes);
private:

    std::unique_ptr<BVHNode> BuildRecursive(std::span<Instance> shapes) const;

    std::unique_ptr<BVHNode> root_;
};



#endif //BVH_H


