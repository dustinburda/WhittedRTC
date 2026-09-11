#include "gtest/gtest.h"

#include "../include/ProjectiveCamera.h"
#include "../include/Util.h"

TEST(ProjectiveCameraTest, CenterPixelRayPointsForward) {
    ProjectiveCamera camera{
        Point3D{0.0, 0.0, 0.0},
        Point3D{0.0, 0.0, 1.0},
        Vec3D{0.0, 1.0, 0.0},
        pi / 2.0
    };
    ImagePlane image_plane{1, 1};

    auto ray = camera.GetRayAt(0, 0, image_plane);

    EXPECT_NEAR(ray.Origin()[0], 0.0, 1e-5);
    EXPECT_NEAR(ray.Origin()[1], 0.0, 1e-5);
    EXPECT_NEAR(ray.Origin()[2], 0.0, 1e-5);

    EXPECT_NEAR(ray.Direction()[0], 0.0, 1e-5);
    EXPECT_NEAR(ray.Direction()[1], 0.0, 1e-5);
    EXPECT_NEAR(ray.Direction()[2], 1.0, 1e-5);
}

TEST(ProjectiveCameraTest, TopLeftPixelRayPointsLeftUpAndForward) {
    ProjectiveCamera camera{
        Point3D{0.0, 0.0, 0.0},
        Point3D{0.0, 0.0, 1.0},
        Vec3D{0.0, 1.0, 0.0},
        pi / 2.0
    };
    ImagePlane image_plane{2, 2};

    auto ray = camera.GetRayAt(0, 0, image_plane);

    EXPECT_LT(ray.Direction()[0], 0.0);
    EXPECT_GT(ray.Direction()[1], 0.0);
    EXPECT_GT(ray.Direction()[2], 0.0);
}

TEST(ProjectiveCameraTest, CameraTransformMovesRayToWorldSpace) {
    ProjectiveCamera camera{
        Point3D{10.0, 20.0, 30.0},
        Point3D{10.0, 20.0, 31.0},
        Vec3D{0.0, 1.0, 0.0},
        pi / 2.0
    };
    ImagePlane image_plane{1, 1};

    auto ray = camera.GetRayAt(0, 0, image_plane);

    EXPECT_NEAR(ray.Origin()[0], 10.0, 1e-5);
    EXPECT_NEAR(ray.Origin()[1], 20.0, 1e-5);
    EXPECT_NEAR(ray.Origin()[2], 30.0, 1e-5);

    EXPECT_NEAR(ray.Direction()[0], 0.0, 1e-5);
    EXPECT_NEAR(ray.Direction()[1], 0.0, 1e-5);
    EXPECT_NEAR(ray.Direction()[2], 1.0, 1e-5);
}
