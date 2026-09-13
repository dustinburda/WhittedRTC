//
// Created by Dustin on 9/12/26.
//

#include <gtest/gtest.h>

#include "../include/BVH.h"
#include "../include/Instance.h"
#include "../include/Ray.h"
#include "../include/ShadeContext.h"
#include "../include/Sphere.h"

TEST(BVHTest, EmptyBVHReturnsNoHit) {
    BVH bvh;
    Ray ray{{0.0, 0.0, -3.0}, {0.0, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_FALSE(bvh.Hit(ray, context));
}

TEST(BVHTest, RayMissingAllInstancesReturnsNoHit) {
    auto left_sphere = std::make_shared<Sphere>(Point3D{-3.0, 0.0, 0.0}, 1.0);
    auto right_sphere = std::make_shared<Sphere>(Point3D{3.0, 0.0, 0.0}, 1.0);

    std::vector<Instance> instances{
        Instance{left_sphere, nullptr, InstanceType::Sphere},
        Instance{right_sphere, nullptr, InstanceType::Sphere}
    };

    BVH bvh{instances};
    Ray ray{{0.0, 3.0, -5.0}, {0.0, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_FALSE(bvh.Hit(ray, context));
}
