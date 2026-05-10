#include <gtest/gtest.h>
#include "triangulation.hpp"

#include <vector>

using namespace geometry;

TEST(TriangulationTest, ReturnsEmptyForLessThanThreePoints)
{
    std::vector<Point2D> points{
        {0.0, 0.0},
        {1.0, 1.0}
    };

    auto triangels = triangulation::DelaunayTriangulation(points);

    EXPECT_TRUE(triangels.empty());
}

TEST(TriangulationTest, BuildsOneTriangleForThreePoints)
{
    std::vector<Point2D> points{
        {0.0, 0.0},
        {10.0, 0.0},
        {0.0, 10.0}
    };

    auto triangle = triangulation::DelaunayTriangulation(points);

    ASSERT_EQ(triangle.size(), 1);
}

TEST(TriangulationTest, BuildsTrianglesForSeveralPoints)
{
    std::vector<Point2D> points{
        {0.0, 0.0},
        {10.0, 0.0},
        {5.0, 8.0},
        {15.0, 5.0},
        {2.0, 12.0}
    };

    auto triangles = triangulation::DelaunayTriangulation(points);

    EXPECT_FALSE(triangles.empty());
}
