//
// Created by Dustin on 9/10/26.
//

#include <gtest/gtest.h>

#include "../include/OrthnormalBasis.h"
#include "../include/Util.h"

TEST(OrthonormalBasisTest, ForwardUpConstructorBuildsDirectXStyleBasis) {
    OrthonormalBasis basis{Vec3D{0.0, 0.0, 1.0}, Vec3D{0.0, 1.0, 0.0}};

    EXPECT_NEAR(basis.e1_[0], 1.0, 1e-5);
    EXPECT_NEAR(basis.e1_[1], 0.0, 1e-5);
    EXPECT_NEAR(basis.e1_[2], 0.0, 1e-5);

    EXPECT_NEAR(basis.e2_[0], 0.0, 1e-5);
    EXPECT_NEAR(basis.e2_[1], 1.0, 1e-5);
    EXPECT_NEAR(basis.e2_[2], 0.0, 1e-5);

    EXPECT_NEAR(basis.e3_[0], 0.0, 1e-5);
    EXPECT_NEAR(basis.e3_[1], 0.0, 1e-5);
    EXPECT_NEAR(basis.e3_[2], 1.0, 1e-5);
}

TEST(OrthonormalBasisTest, ForwardUpConstructorNormalizesInput) {
    OrthonormalBasis basis{Vec3D{0.0, 0.0, 5.0}, Vec3D{0.0, 2.0, 0.0}};

    EXPECT_NEAR(basis.e1_.Length(), 1.0, 1e-5);
    EXPECT_NEAR(basis.e2_.Length(), 1.0, 1e-5);
    EXPECT_NEAR(basis.e3_.Length(), 1.0, 1e-5);

    EXPECT_NEAR(Dot(basis.e1_, basis.e2_), 0.0, 1e-5);
    EXPECT_NEAR(Dot(basis.e1_, basis.e3_), 0.0, 1e-5);
    EXPECT_NEAR(Dot(basis.e2_, basis.e3_), 0.0, 1e-5);
}

TEST(OrthonormalBasisTest, ForwardUpConstructorOrthogonalizesApproximateUp) {
    OrthonormalBasis basis{Vec3D{0.0, 0.0, 1.0}, Vec3D{0.0, 1.0, 1.0}};

    EXPECT_NEAR(basis.e1_.Length(), 1.0, 1e-5);
    EXPECT_NEAR(basis.e2_.Length(), 1.0, 1e-5);
    EXPECT_NEAR(basis.e3_.Length(), 1.0, 1e-5);

    EXPECT_NEAR(Dot(basis.e1_, basis.e2_), 0.0, 1e-5);
    EXPECT_NEAR(Dot(basis.e1_, basis.e3_), 0.0, 1e-5);
    EXPECT_NEAR(Dot(basis.e2_, basis.e3_), 0.0, 1e-5);

    EXPECT_NEAR(basis.e1_[0], 1.0, 1e-5);
    EXPECT_NEAR(basis.e1_[1], 0.0, 1e-5);
    EXPECT_NEAR(basis.e1_[2], 0.0, 1e-5);

    EXPECT_NEAR(basis.e2_[0], 0.0, 1e-5);
    EXPECT_NEAR(basis.e2_[1], 1.0, 1e-5);
    EXPECT_NEAR(basis.e2_[2], 0.0, 1e-5);

    EXPECT_NEAR(basis.e3_[0], 0.0, 1e-5);
    EXPECT_NEAR(basis.e3_[1], 0.0, 1e-5);
    EXPECT_NEAR(basis.e3_[2], 1.0, 1e-5);
}

TEST(OrthonormalBasisTest, DirectBasisConstructorNormalizesAxes) {
    OrthonormalBasis basis{
        Vec3D{2.0, 0.0, 0.0},
        Vec3D{0.0, 3.0, 0.0},
        Vec3D{0.0, 0.0, 4.0}
    };

    EXPECT_NEAR(basis.e1_[0], 1.0, 1e-5);
    EXPECT_NEAR(basis.e2_[1], 1.0, 1e-5);
    EXPECT_NEAR(basis.e3_[2], 1.0, 1e-5);
}
