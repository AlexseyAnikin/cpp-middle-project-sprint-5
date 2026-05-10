#include <gtest/gtest.h>
#include "convex_hull.hpp"

#include <algorithm>
#include <vector>

using namespace geometry;

namespace{

    bool ContainsPoint(const std::vector<Point2D>& points, const Point2D& target)
    {
        return std::ranges::any_of(points, [&](const Point2D& point)
        {
            return point.x == target.x && point.y == target.y;
        });
    }
}

TEST(ConvexHullTest, ReturnsEmptyForLessThanThreePoints)
{
    std::vector<Point2D> points{{0.0, 0.0}, {1.0, 1.0}};

    auto null = convex_hull::GrahamScan(points);

    EXPECT_TRUE(null.empty());
}

TEST(ConvexHullTest, BuildsHullForTriangle)
{
    std::vector<Point2D> points{
        {0.0, 0.0},
        {10.0, 0.0},
        {5.0, 10.0}
    };

    auto null = convex_hull::GrahamScan(points);
    
    EXPECT_EQ(null.size(), 3);
    EXPECT_TRUE(ContainsPoint(null, {0.0, 0.0}));
    EXPECT_TRUE(ContainsPoint(null, {10.0, 0.0}));
    EXPECT_TRUE(ContainsPoint(null, {5.0, 10.0}));
}

TEST(ConvexHullTest, IgnoresInnerPoint)
{
    std::vector<Point2D> points{
        {0.0, 0.0},
        {10.0, 0.0},
        {10.0, 10.0},
        {0.0, 10.0},
        {5.0, 5.0}
    };

    auto hull = convex_hull::GrahamScan(points);

    EXPECT_EQ(hull.size(), 4);
    EXPECT_TRUE(ContainsPoint(hull, {0.0, 0.0}));
    EXPECT_TRUE(ContainsPoint(hull, {10.0, 0.0}));
    EXPECT_TRUE(ContainsPoint(hull, {10.0, 10.0}));
    EXPECT_TRUE(ContainsPoint(hull, {0.0, 10.0}));
    EXPECT_FALSE(ContainsPoint(hull, {5.0, 5.0}));
}
