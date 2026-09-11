//
// Created by Dustin on 2/2/26.
//

#include <gtest/gtest.h>

#include "../include/Point.h"
#include "../include/Vector.h"

TEST(PointTest, DefaultConstructor) {
    Point<double, 4> point;

    EXPECT_EQ(point[0], 0.0);
    EXPECT_EQ(point[1], 0.0);
    EXPECT_EQ(point[2], 0.0);
    EXPECT_EQ(point[3], 0.0);
}

TEST(PointTest, InitializerListConstructor) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    EXPECT_EQ(point[0], 1.0);
    EXPECT_EQ(point[1], 2.0);
    EXPECT_EQ(point[2], 3.0);
    EXPECT_EQ(point[3], 4.0);
}

TEST(PointTest, InitializerListConstructorThrow) {
    EXPECT_THROW((Point<double, 4>{1.0, 2.0, 3.0}), std::logic_error);
    EXPECT_THROW((Point<double, 2>{1.0, 2.0, 3.0}), std::logic_error);
}

TEST(PointTest, SubscriptOperator) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    EXPECT_EQ(point[0], 1.0);
    EXPECT_EQ(point[1], 2.0);
    EXPECT_EQ(point[2], 3.0);
    EXPECT_EQ(point[3], 4.0);

    point[3] = 5.0;
    EXPECT_EQ(point[3], 5.0);

    EXPECT_THROW((point[4]), std::logic_error);

    const Point<double, 4> c_point{1.0, 2.0, 3.0, 4.0};
    EXPECT_EQ(c_point[0], 1.0);
    EXPECT_EQ(c_point[1], 2.0);
}

TEST(PointTest, Negation) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    auto negative = -point;

    EXPECT_EQ(negative[0], -1.0);
    EXPECT_EQ(negative[1], -2.0);
    EXPECT_EQ(negative[2], -3.0);
    EXPECT_EQ(negative[3], -4.0);
}

TEST(PointTest, HomogenousPointToCartesianVectorDropsLastCoordinate) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    auto vector = point.HomogenousPointToCartesianVector();

    EXPECT_EQ(vector[0], 1.0);
    EXPECT_EQ(vector[1], 2.0);
    EXPECT_EQ(vector[2], 3.0);
    EXPECT_EQ(vector[3], 0.0);
}

TEST(PointTest, ToVectorCopiesAllPoint3DCoordinates) {
    Point3D point{1.0, 2.0, 3.0};

    auto vector = point.ToVector();

    EXPECT_EQ(vector[0], 1.0);
    EXPECT_EQ(vector[1], 2.0);
    EXPECT_EQ(vector[2], 3.0);
}

TEST(PointTest, ToVectorCopiesAllPoint2DCoordinates) {
    Point2D point{1.0, 2.0};

    auto vector = point.ToVector();

    EXPECT_EQ(vector[0], 1.0);
    EXPECT_EQ(vector[1], 2.0);
}

TEST(PointTest, ToVectorCopiesAllPoint4DCoordinates) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    auto vector = point.ToVector();

    EXPECT_EQ(vector[0], 1.0);
    EXPECT_EQ(vector[1], 2.0);
    EXPECT_EQ(vector[2], 3.0);
    EXPECT_EQ(vector[3], 4.0);
}

TEST(PointTest, ToString) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    EXPECT_EQ(point.toString(), "Point: [ 1,  2,  3,  4]");
}

TEST(PointTest, PointVectorAddition) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};
    Vector<double, 4> vector{1.0, 1.0, 1.0, 1.0};

    auto translated = point + vector;

    EXPECT_EQ(translated[0], 2.0);
    EXPECT_EQ(translated[1], 3.0);
    EXPECT_EQ(translated[2], 4.0);
    EXPECT_EQ(translated[3], 5.0);
}

TEST(PointTest, PointVectorSubtraction) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};
    Vector<double, 4> vector{1.0, 1.0, 1.0, 1.0};

    auto translated = point - vector;

    EXPECT_EQ(translated[0], 0.0);
    EXPECT_EQ(translated[1], 1.0);
    EXPECT_EQ(translated[2], 2.0);
    EXPECT_EQ(translated[3], 3.0);
}

TEST(PointTest, PointSubtraction) {
    Point<double, 4> point1{1.0, 2.0, 3.0, 4.0};
    Point<double, 4> point2{1.0, 1.0, 1.0, 1.0};

    auto difference = point1 - point2;

    EXPECT_EQ(difference[0], 0.0);
    EXPECT_EQ(difference[1], 1.0);
    EXPECT_EQ(difference[2], 2.0);
    EXPECT_EQ(difference[3], 3.0);
}

TEST(PointTest, ScalarMultiplication) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    auto point_t_left = point * 2.0;
    EXPECT_EQ(point_t_left[0], 2.0);
    EXPECT_EQ(point_t_left[1], 4.0);
    EXPECT_EQ(point_t_left[2], 6.0);
    EXPECT_EQ(point_t_left[3], 8.0);

    auto point_t_right = 2.0 * point;
    EXPECT_EQ(point_t_right[0], 2.0);
    EXPECT_EQ(point_t_right[1], 4.0);
    EXPECT_EQ(point_t_right[2], 6.0);
    EXPECT_EQ(point_t_right[3], 8.0);
}

TEST(PointTest, ScalarDivision) {
    Point<double, 4> point{1.0, 2.0, 3.0, 4.0};

    auto quotient = point / 2.0;

    EXPECT_EQ(quotient[0], 0.5);
    EXPECT_EQ(quotient[1], 1.0);
    EXPECT_EQ(quotient[2], 1.5);
    EXPECT_EQ(quotient[3], 2.0);
}

TEST(PointTest, DistanceSquared) {
    Point<double, 4> point1{1.0, 2.0, 3.0, 4.0};
    Point<double, 4> point2{0.0, 0.0, 0.0, 0.0};

    EXPECT_EQ(DistanceSquared(point1, point2), 30.0);
}

TEST(PointTest, Distance) {
    Point<double, 4> point1{1.0, 2.0, 3.0, 4.0};
    Point<double, 4> point2{0.0, 0.0, 0.0, 0.0};

    EXPECT_NEAR(Distance(point1, point2), std::sqrt(30.0), 1e-5);
}
