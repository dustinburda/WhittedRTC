#include "gtest/gtest.h"

#include "../include/OrthographicCamera.h"

TEST(OrthographicCameraTest, AllRaysPointForward) {
    OrthographicCamera camera{
        Point3D{0.0, 0.0, 0.0},
        Point3D{0.0, 0.0, 1.0},
        Vec3D{0.0, 1.0, 0.0}
    };
    ImagePlane image_plane{2, 2};

    auto top_left_ray = camera.GetRayAt(0, 0, image_plane);
    auto bottom_right_ray = camera.GetRayAt(1, 1, image_plane);

    EXPECT_NEAR(top_left_ray.Direction()[0], 0.0, 1e-5);
    EXPECT_NEAR(top_left_ray.Direction()[1], 0.0, 1e-5);
    EXPECT_NEAR(top_left_ray.Direction()[2], 1.0, 1e-5);

    EXPECT_NEAR(bottom_right_ray.Direction()[0], 0.0, 1e-5);
    EXPECT_NEAR(bottom_right_ray.Direction()[1], 0.0, 1e-5);
    EXPECT_NEAR(bottom_right_ray.Direction()[2], 1.0, 1e-5);
}

TEST(OrthographicCameraTest, RayOriginsVaryAcrossImagePlane) {
    OrthographicCamera camera{
        Point3D{0.0, 0.0, 0.0},
        Point3D{0.0, 0.0, 1.0},
        Vec3D{0.0, 1.0, 0.0}
    };
    ImagePlane image_plane{2, 2};

    auto top_left_ray = camera.GetRayAt(0, 0, image_plane);
    auto bottom_right_ray = camera.GetRayAt(1, 1, image_plane);

    EXPECT_NEAR(top_left_ray.Origin()[0], -0.5, 1e-5);
    EXPECT_NEAR(top_left_ray.Origin()[1], 0.5, 1e-5);
    EXPECT_NEAR(top_left_ray.Origin()[2], 1.0, 1e-5);

    EXPECT_NEAR(bottom_right_ray.Origin()[0], 0.5, 1e-5);
    EXPECT_NEAR(bottom_right_ray.Origin()[1], -0.5, 1e-5);
    EXPECT_NEAR(bottom_right_ray.Origin()[2], 1.0, 1e-5);
}

TEST(OrthographicCameraTest, CameraTransformMovesRayOriginToWorldSpace) {
    OrthographicCamera camera{
        Point3D{10.0, 20.0, 30.0},
        Point3D{10.0, 20.0, 31.0},
        Vec3D{0.0, 1.0, 0.0}
    };
    ImagePlane image_plane{1, 1};

    auto ray = camera.GetRayAt(0, 0, image_plane);

    EXPECT_NEAR(ray.Origin()[0], 10.0, 1e-5);
    EXPECT_NEAR(ray.Origin()[1], 20.0, 1e-5);
    EXPECT_NEAR(ray.Origin()[2], 31.0, 1e-5);

    EXPECT_NEAR(ray.Direction()[0], 0.0, 1e-5);
    EXPECT_NEAR(ray.Direction()[1], 0.0, 1e-5);
    EXPECT_NEAR(ray.Direction()[2], 1.0, 1e-5);
}
