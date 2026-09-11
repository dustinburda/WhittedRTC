//
// Created by Dustin on 12/2/24.
//

#include <gtest/gtest.h>

#include "../include/Normal.h"
#include "../include/Point.h"
#include "../include/Ray.h"
#include "../include/Transformation.h"
#include "../include/Util.h"
#include "../include/Vector.h"

TEST(TransformationTest, IdentityTransformVector) {
    auto transformation = Transformation::Identity();
    Vector<double, 3> vector{1.0, 2.0, 3.0};

    auto transformed = transformation(vector);

    EXPECT_EQ(transformed, vector);
}

TEST(TransformationTest, IdentityTransformPoint) {
    auto transformation = Transformation::Identity();
    Point<double, 3> point{1.0, 2.0, 3.0};

    auto transformed = transformation(point);

    EXPECT_EQ(transformed[0], 1.0);
    EXPECT_EQ(transformed[1], 2.0);
    EXPECT_EQ(transformed[2], 3.0);
}

TEST(TransformationTest, IdentityTransformNormal) {
    auto transformation = Transformation::Identity();
    Normal<double, 3> normal{1.0, 2.0, 3.0};

    auto transformed = transformation(normal);

    EXPECT_EQ(transformed[0], 1.0);
    EXPECT_EQ(transformed[1], 2.0);
    EXPECT_EQ(transformed[2], 3.0);
}

TEST(TransformationTest, TranslationTransformsPoint) {
    auto transformation = Transformation::Translation(3.0, 4.0, 5.0);
    Point<double, 3> point{1.0, 2.0, 3.0};

    auto transformed = transformation(point);

    EXPECT_EQ(transformed[0], 4.0);
    EXPECT_EQ(transformed[1], 6.0);
    EXPECT_EQ(transformed[2], 8.0);
}

TEST(TransformationTest, TranslationDoesNotTransformVector) {
    auto transformation = Transformation::Translation(3.0, 4.0, 5.0);
    Vector<double, 3> vector{1.0, 2.0, 3.0};

    auto transformed = transformation(vector);

    EXPECT_EQ(transformed, vector);
}

TEST(TransformationTest, TranslationDoesNotTransformNormal) {
    auto transformation = Transformation::Translation(3.0, 4.0, 5.0);
    Normal<double, 3> normal{1.0, 2.0, 3.0};

    auto transformed = transformation(normal);

    EXPECT_EQ(transformed[0], 1.0);
    EXPECT_EQ(transformed[1], 2.0);
    EXPECT_EQ(transformed[2], 3.0);
}

TEST(TransformationTest, ScaleTransformsPoint) {
    auto transformation = Transformation::Scale(2.0, 3.0, 4.0);
    Point<double, 3> point{1.0, 2.0, 3.0};

    auto transformed = transformation(point);

    EXPECT_EQ(transformed[0], 2.0);
    EXPECT_EQ(transformed[1], 6.0);
    EXPECT_EQ(transformed[2], 12.0);
}

TEST(TransformationTest, ScaleTransformsVector) {
    auto transformation = Transformation::Scale(2.0, 3.0, 4.0);
    Vector<double, 3> vector{1.0, 2.0, 3.0};

    auto transformed = transformation(vector);

    EXPECT_EQ(transformed[0], 2.0);
    EXPECT_EQ(transformed[1], 6.0);
    EXPECT_EQ(transformed[2], 12.0);
}

TEST(TransformationTest, ScaleTransformsNormalWithInverseTranspose) {
    auto transformation = Transformation::Scale(2.0, 3.0, 4.0);
    Normal<double, 3> normal{1.0, 2.0, 3.0};

    auto transformed = transformation(normal);

    EXPECT_NEAR(transformed[0], 0.5, 1e-5);
    EXPECT_NEAR(transformed[1], 2.0 / 3.0, 1e-5);
    EXPECT_NEAR(transformed[2], 0.75, 1e-5);
}

TEST(TransformationTest, RotationXTransformsPoint) {
    auto transformation = Transformation::RotationX(pi / 2.0);
    Point<double, 3> point{0.0, 1.0, 0.0};

    auto transformed = transformation(point);

    EXPECT_NEAR(transformed[0], 0.0, 1e-5);
    EXPECT_NEAR(transformed[1], 0.0, 1e-5);
    EXPECT_NEAR(transformed[2], 1.0, 1e-5);
}

TEST(TransformationTest, RotationYTransformsVector) {
    auto transformation = Transformation::RotationY(pi / 2.0);
    Vector<double, 3> vector{0.0, 0.0, 1.0};

    auto transformed = transformation(vector);

    EXPECT_NEAR(transformed[0], 1.0, 1e-5);
    EXPECT_NEAR(transformed[1], 0.0, 1e-5);
    EXPECT_NEAR(transformed[2], 0.0, 1e-5);
}

TEST(TransformationTest, RotationZTransformsNormal) {
    auto transformation = Transformation::RotationZ(pi / 2.0);
    Normal<double, 3> normal{1.0, 0.0, 0.0};

    auto transformed = transformation(normal);

    EXPECT_NEAR(transformed[0], 0.0, 1e-5);
    EXPECT_NEAR(transformed[1], 1.0, 1e-5);
    EXPECT_NEAR(transformed[2], 0.0, 1e-5);
}

TEST(TransformationTest, ComposedTransformAppliesRightHandSideFirst) {
    auto transformation = Transformation::Translation(5.0, 0.0, 0.0) *
                          Transformation::Scale(2.0, 2.0, 2.0);
    Point<double, 3> point{1.0, 1.0, 1.0};

    auto transformed = transformation(point);

    EXPECT_EQ(transformed[0], 7.0);
    EXPECT_EQ(transformed[1], 2.0);
    EXPECT_EQ(transformed[2], 2.0);
}

TEST(TransformationTest, WorldToLocalTransformsPointIntoBasisCoordinates) {
    auto transformation = Transformation::WorldToLocal(
        Point3D{10.0, 20.0, 30.0},
        OrthonormalBasis{
            Vec3D{1.0, 0.0, 0.0},
            Vec3D{0.0, 1.0, 0.0},
            Vec3D{0.0, 0.0, 1.0}
        }
    );
    Point3D point{11.0, 22.0, 33.0};

    auto transformed = transformation(point);

    EXPECT_NEAR(transformed[0], 1.0, 1e-5);
    EXPECT_NEAR(transformed[1], 2.0, 1e-5);
    EXPECT_NEAR(transformed[2], 3.0, 1e-5);
}

TEST(TransformationTest, WorldToLocalInverseTransformsLocalPointBackToWorld) {
    auto transformation = Transformation::WorldToLocal(
        Point3D{10.0, 20.0, 30.0},
        OrthonormalBasis{
            Vec3D{1.0, 0.0, 0.0},
            Vec3D{0.0, 1.0, 0.0},
            Vec3D{0.0, 0.0, 1.0}
        }
    );
    Point3D point{1.0, 2.0, 3.0};

    auto transformed = transformation.ApplyInverse(point);

    EXPECT_NEAR(transformed[0], 11.0, 1e-5);
    EXPECT_NEAR(transformed[1], 22.0, 1e-5);
    EXPECT_NEAR(transformed[2], 33.0, 1e-5);
}

TEST(TransformationTest, WorldToLocalUsesBasisRows) {
    auto transformation = Transformation::WorldToLocal(
        Point3D{1.0, 2.0, 3.0},
        OrthonormalBasis{
            Vec3D{0.0, 1.0, 0.0},
            Vec3D{0.0, 0.0, 1.0},
            Vec3D{1.0, 0.0, 0.0}
        }
    );
    Point3D point{4.0, 6.0, 8.0};

    auto transformed = transformation(point);

    EXPECT_NEAR(transformed[0], 4.0, 1e-5);
    EXPECT_NEAR(transformed[1], 5.0, 1e-5);
    EXPECT_NEAR(transformed[2], 3.0, 1e-5);
}

TEST(TransformationTest, ApplyInverseToPoint) {
    auto transformation = Transformation::Translation(3.0, 4.0, 5.0);
    Point<double, 3> point{4.0, 6.0, 8.0};

    auto transformed = transformation.ApplyInverse(point);

    EXPECT_EQ(transformed[0], 1.0);
    EXPECT_EQ(transformed[1], 2.0);
    EXPECT_EQ(transformed[2], 3.0);
}

TEST(TransformationTest, ApplyInverseToVector) {
    auto transformation = Transformation::Scale(2.0, 3.0, 4.0);
    Vector<double, 3> vector{2.0, 6.0, 12.0};

    auto transformed = transformation.ApplyInverse(vector);

    EXPECT_NEAR(transformed[0], 1.0, 1e-5);
    EXPECT_NEAR(transformed[1], 2.0, 1e-5);
    EXPECT_NEAR(transformed[2], 3.0, 1e-5);
}

TEST(TransformationTest, ApplyInverseToNormalUsesForwardTranspose) {
    auto transformation = Transformation::Scale(2.0, 3.0, 4.0);
    Normal<double, 3> normal{1.0, 2.0, 3.0};

    auto transformed = transformation.ApplyInverse(normal);

    EXPECT_NEAR(transformed[0], 2.0, 1e-5);
    EXPECT_NEAR(transformed[1], 6.0, 1e-5);
    EXPECT_NEAR(transformed[2], 12.0, 1e-5);
}

TEST(TransformationTest, TransformRay) {
    auto transformation = Transformation::Translation(3.0, 4.0, 5.0);
    Ray ray{Point<double, 3>{1.0, 2.0, 3.0}, Vector<double, 3>{1.0, 0.0, 0.0}, RayType::PrimaryRay};

    auto transformed = transformation(ray);

    EXPECT_EQ(transformed.Origin()[0], 4.0);
    EXPECT_EQ(transformed.Origin()[1], 6.0);
    EXPECT_EQ(transformed.Origin()[2], 8.0);
    EXPECT_EQ(transformed.Direction(), (Vector<double, 3>{1.0, 0.0, 0.0}));
}
