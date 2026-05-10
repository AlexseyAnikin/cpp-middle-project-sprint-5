#include <gtest/gtest.h>

#include "geometry.hpp"
#include "queries.hpp"

using namespace geometry;

TEST(QueriesTest, GetHeightReturnsShapeHeight) 
{
    Shape circle = Circle{{0.0, 0.0}, 5.0};

    EXPECT_DOUBLE_EQ(queries::GetHeight(circle), 5.0);
}

TEST(QueriesTest, GetBoundBoxReturnsShapeBoundingBox) 
{
    Shape circle = Circle{{10.0, 20.0}, 5.0};

    auto box = queries::GetBoundBox(circle);

    EXPECT_DOUBLE_EQ(box.min_x, 5.0);
    EXPECT_DOUBLE_EQ(box.min_y, 15.0);
    EXPECT_DOUBLE_EQ(box.max_x, 15.0);
    EXPECT_DOUBLE_EQ(box.max_y, 25.0);
}

TEST(QueriesTest, BoundingBoxesOverlapReturnsTrueForOverlappingShapes) 
{
    Shape lhs = Circle{{0.0, 0.0}, 5.0};
    Shape rhs = Circle{{3.0, 0.0}, 5.0};

    EXPECT_TRUE(queries::BoundingBoxesOverlap(lhs, rhs));
}

TEST(QueriesTest, BoundingBoxesOverlapReturnsFalseForSeparatedShapes) 
{
    Shape lhs = Circle{{0.0, 0.0}, 1.0};
    Shape rhs = Circle{{100.0, 100.0}, 1.0};

    EXPECT_FALSE(queries::BoundingBoxesOverlap(lhs, rhs));
}

TEST(QueriesTest, DistanceToPointWorksForCircle) 
{
    Shape circle = Circle{{0.0, 0.0}, 5.0};
    Point2D point{10.0, 0.0};

    EXPECT_DOUBLE_EQ(queries::DistanceToPoint(circle, point), 5.0);
}

TEST(QueriesTest, DistanceToPointWorksForLine) 
{
    Shape line = Line{{0.0, 0.0}, {10.0, 0.0}};
    Point2D point{5.0, 5.0};

    EXPECT_DOUBLE_EQ(queries::DistanceToPoint(line, point), 5.0);
}

TEST(QueriesTest, DistanceBetweenSameShapeTypesReturnsValue) 
{
    Shape lhs = Circle{{0.0, 0.0}, 1.0};
    Shape rhs = Circle{{5.0, 0.0}, 1.0};

    auto distance = queries::DistanceBetweenShapes(lhs, rhs);

    ASSERT_TRUE(distance.has_value());
    EXPECT_DOUBLE_EQ(*distance, 3.0);
}

TEST(QueriesTest, DistanceBetweenUnsupportedShapeTypesReturnsNullopt) 
{
    Shape lhs = Circle{{0.0, 0.0}, 1.0};
    Shape rhs = Triangle{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};

    auto distance = queries::DistanceBetweenShapes(lhs, rhs);

    EXPECT_FALSE(distance.has_value());
}