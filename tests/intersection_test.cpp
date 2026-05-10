#include <gtest/gtest.h>

#include "geometry.hpp"
#include "intersections.hpp"

using namespace geometry;

TEST(IntersectionsTest, LineLineIntersectionReturnsPoint) 
{
    Shape lhs = Line{{0.0, 0.0}, {10.0, 10.0}};
    Shape rhs = Line{{0.0, 10.0}, {10.0, 0.0}};

    auto point = intersections::GetIntersectPoint(lhs, rhs);

    ASSERT_TRUE(point.has_value());

    EXPECT_NEAR(point->x, 5.0, 1e-9);
    EXPECT_NEAR(point->y, 5.0, 1e-9);
}

TEST(IntersectionsTest, ParallelLinesReturnNullopt) 
{
    Shape lhs = Line{{0.0, 0.0}, {10.0, 0.0}};
    Shape rhs = Line{{0.0, 5.0}, {10.0, 5.0}};

    auto point = intersections::GetIntersectPoint(lhs, rhs);

    EXPECT_FALSE(point.has_value());
}

TEST(IntersectionsTest, CircleCircleIntersectionReturnsPoint) 
{
    Shape lhs = Circle{{0.0, 0.0}, 5.0};
    Shape rhs = Circle{{8.0, 0.0}, 5.0};

    auto point = intersections::GetIntersectPoint(lhs, rhs);

    ASSERT_TRUE(point.has_value());

    EXPECT_NEAR(point->x, 4.0, 1e-9);
}

TEST(IntersectionsTest, DistantCirclesReturnNullopt) 
{
    Shape lhs = Circle{{0.0, 0.0}, 1.0};
    Shape rhs = Circle{{100.0, 100.0}, 1.0};

    auto point = intersections::GetIntersectPoint(lhs, rhs);

    EXPECT_FALSE(point.has_value());
}

TEST(IntersectionsTest, LineCircleIntersectionReturnsPoint) 
{
    Shape line = Line{{-10.0, 0.0}, {10.0, 0.0}};
    Shape circle = Circle{{0.0, 0.0}, 5.0};

    auto point = intersections::GetIntersectPoint(line, circle);

    ASSERT_TRUE(point.has_value());

    EXPECT_NEAR(point->y, 0.0, 1e-9);
}

TEST(IntersectionsTest, UnsupportedShapesThrowLogicError) 
{
    Shape lhs = Triangle{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
    Shape rhs = Rectangle{{0.0, 0.0}, 5.0, 5.0};

    EXPECT_THROW(
        intersections::GetIntersectPoint(lhs, rhs),
        std::logic_error
    );
}