//
// Created by Dustin on 9/12/26.
//

#include <gtest/gtest.h>

#include "../include/BoundingBox.h"
#include "../include/Ray.h"
#include "../include/ShadeContext.h"

TEST(BoundingBoxTest, ConstructorOrdersMinAndMaxCoordinates) {
    BoundingBox box{{3.0, -2.0, 5.0}, {-1.0, 4.0, 2.0}};

    EXPECT_DOUBLE_EQ(box.Min()[0], -1.0);
    EXPECT_DOUBLE_EQ(box.Min()[1], -2.0);
    EXPECT_DOUBLE_EQ(box.Min()[2], 2.0);
    EXPECT_DOUBLE_EQ(box.Max()[0], 3.0);
    EXPECT_DOUBLE_EQ(box.Max()[1], 4.0);
    EXPECT_DOUBLE_EQ(box.Max()[2], 5.0);
}

TEST(BoundingBoxTest, RayHitsBoxFromOutside) {
    BoundingBox box{{-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}};
    Ray ray{{0.0, 0.0, -3.0}, {0.25, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_TRUE(box.Hit(ray, context));
}

TEST(BoundingBoxTest, RayMissesBoxFromOutside) {
    BoundingBox box{{-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}};
    Ray ray{{2.0, 0.0, -3.0}, {0.0, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_FALSE(box.Hit(ray, context));
}

TEST(BoundingBoxTest, RayStartingInsideBoxHits) {
    BoundingBox box{{-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}};
    Ray ray{{0.0, 0.0, 0.0}, {1.0, 0.5, 0.25}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_TRUE(box.Hit(ray, context));
}

TEST(BoundingBoxTest, RayPointingAwayFromBoxMisses) {
    BoundingBox box{{-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}};
    Ray ray{{0.0, 0.0, 3.0}, {0.0, 0.0, 1.0}, RayType::PrimaryRay};
    ShadeContext context;

    EXPECT_FALSE(box.Hit(ray, context));
}
