//
// Created by Dustin on 11/26/24.
//
#include <gtest/gtest.h>

#include "../include/Ray.h"
#include "../include/ShadeContext.h"
#include "../include/Sphere.h"

TEST(SphereTest, RayHitsSphereAtClosestPositiveIntersection) {
    Sphere sphere{{0.0, 0.0, 0.0}, 1.0};
    Ray ray{{0.0, 0.0, -3.0}, {0.0, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_TRUE(sphere.Hit(ray, context));
    EXPECT_NEAR(context.t_max_, 2.0, 1e-5);
    EXPECT_NEAR(context.point_[0], 0.0, 1e-5);
    EXPECT_NEAR(context.point_[1], 0.0, 1e-5);
    EXPECT_NEAR(context.point_[2], -1.0, 1e-5);
    EXPECT_NEAR(context.normal_[0], 0.0, 1e-5);
    EXPECT_NEAR(context.normal_[1], 0.0, 1e-5);
    EXPECT_NEAR(context.normal_[2], -1.0, 1e-5);
}

TEST(SphereTest, RayMissesSphere) {
    Sphere sphere{{0.0, 0.0, 0.0}, 1.0};
    Ray ray{{0.0, 2.0, -3.0}, {0.0, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_FALSE(sphere.Hit(ray, context));
}

TEST(SphereTest, RayStartingInsideSphereHitsExitPoint) {
    Sphere sphere{{0.0, 0.0, 0.0}, 1.0};
    Ray ray{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_TRUE(sphere.Hit(ray, context));
    EXPECT_NEAR(context.t_max_, 1.0, 1e-5);
    EXPECT_NEAR(context.point_[0], 1.0, 1e-5);
    EXPECT_NEAR(context.point_[1], 0.0, 1e-5);
    EXPECT_NEAR(context.point_[2], 0.0, 1e-5);
    EXPECT_NEAR(context.normal_[0], 1.0, 1e-5);
    EXPECT_NEAR(context.normal_[1], 0.0, 1e-5);
    EXPECT_NEAR(context.normal_[2], 0.0, 1e-5);
}

TEST(SphereTest, RayHitOutsideTBoundsReturnsFalse) {
    Sphere sphere{{0.0, 0.0, 0.0}, 1.0};
    Ray ray{{0.0, 0.0, -3.0}, {0.0, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;
    context.t_min_ = 0.0;
    context.t_max_ = 1.5;

    EXPECT_FALSE(sphere.Hit(ray, context));
}

TEST(SphereTest, BoundingBoxUsesCenterAndRadius) {
    Sphere sphere{{1.0, 2.0, 3.0}, 2.0};

    auto bbox = sphere.BBox();

    EXPECT_DOUBLE_EQ(bbox.Min()[0], -1.0);
    EXPECT_DOUBLE_EQ(bbox.Min()[1], 0.0);
    EXPECT_DOUBLE_EQ(bbox.Min()[2], 1.0);
    EXPECT_DOUBLE_EQ(bbox.Max()[0], 3.0);
    EXPECT_DOUBLE_EQ(bbox.Max()[1], 4.0);
    EXPECT_DOUBLE_EQ(bbox.Max()[2], 5.0);
}
