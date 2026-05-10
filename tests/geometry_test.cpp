#include <gtest/gtest.h>
#include "geometry.hpp"

// #include <format>
// #include <vector>

using namespace geometry;

TEST(Point2DTest, MathOperationsWorkCorrectly)
{
    Point2D a{3.0, 4.0};
    Point2D b{1.0, 2.0};

    auto sum = a + b;
    EXPECT_DOUBLE_EQ(sum.x, 4.0);
    EXPECT_DOUBLE_EQ(sum.y, 6.0);

    auto diff = a - b;
    EXPECT_DOUBLE_EQ(diff.x, 2.0);
    EXPECT_DOUBLE_EQ(diff.y, 2.0);

    EXPECT_DOUBLE_EQ(a.Dot(b), 11.0);
    EXPECT_DOUBLE_EQ(a.Cross(b), 2.0);
    EXPECT_DOUBLE_EQ(a.Length(), 5.0);
}

TEST(Point2DTest, DistanceToWorksCorrectly)
{
    Point2D a{0.0, 0.0};
    Point2D b{3.0, 4.0};

    EXPECT_DOUBLE_EQ(a.DistanceTo(b), 5.0);
}

TEST(Point2DTest, OverlapsWorksCorrectly)
{
    BoundingBox a{0.0, 0.0, 10.0, 10.0};
    BoundingBox b{5.0, 5.0, 15.0, 15.0};
    BoundingBox c{20.0, 20.0, 30.0, 30.0};

    EXPECT_TRUE(a.Overlaps(b));
    EXPECT_FALSE(a.Overlaps(c));
}

TEST(LineTest, BoundBoxHeightAndCenterWorkCorrectly)
{
    Line line{{1.0, 2.0}, {5.0, 8.0}};

    auto box = line.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 1.0);
    EXPECT_DOUBLE_EQ(box.min_y, 2.0);
    EXPECT_DOUBLE_EQ(box.max_x, 5.0);
    EXPECT_DOUBLE_EQ(box.max_y, 8.0);

    EXPECT_DOUBLE_EQ(line.Height(), 8.0);

    auto center = line.Center();
    EXPECT_DOUBLE_EQ(center.x, 3.0);
    EXPECT_DOUBLE_EQ(center.y, 5.0);
}

TEST(TriangleTest, AreaBoundBoxAndCenterWorkCorrectly)
{
    Triangle triangle{{0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0}};

    EXPECT_DOUBLE_EQ(triangle.Area(), 6.0);

    auto box = triangle.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 0.0);
    EXPECT_DOUBLE_EQ(box.min_y, 0.0);
    EXPECT_DOUBLE_EQ(box.max_x, 4.0);
    EXPECT_DOUBLE_EQ(box.max_y, 3.0);

    auto center = triangle.Center();
    EXPECT_NEAR(center.x, 4.0 / 3.0, 1e-9);
    EXPECT_NEAR(center.y, 1.0, 1e-9);
}

TEST(RecetangleTest, BoundBoxVerticesAndCenterWorkCorrectly)
{
    Rectangle react{{1.0, 2.0}, 10.5, 5.0};

    auto box = react.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 1.0);
    EXPECT_DOUBLE_EQ(box.max_y, 7.0);
    EXPECT_DOUBLE_EQ(box.max_x, 11.5);
    EXPECT_DOUBLE_EQ(box.max_y, 7.0);

    EXPECT_DOUBLE_EQ(react.Height(), 7.0);

    auto center = react.Center();
    EXPECT_DOUBLE_EQ(center.x, 6.25);
    EXPECT_DOUBLE_EQ(center.y, 4.5);
}

TEST(CircleTest, BoundBoxHeightAndCenterWorkCorrectly)
{
    Circle circle{{10.0, 20.0}, 5.0};

    auto box = circle.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 5.0);
    EXPECT_DOUBLE_EQ(box.min_y, 15.0);
    EXPECT_DOUBLE_EQ(box.max_x, 15.0);
    EXPECT_DOUBLE_EQ(box.max_y, 25.0);

    EXPECT_DOUBLE_EQ(circle.Height(), 25.0);

    auto center = circle.Center();
    EXPECT_DOUBLE_EQ(center.x, 10.0);
    EXPECT_DOUBLE_EQ(center.y, 20.0);
}

TEST(RegularPolygonTest, CreatesExpectedNumberOfVertices)
{
    RegularPolygon polygon{{0.0, 0.0}, 10.0, 5};

    auto verticals = polygon.Vertices();
    EXPECT_EQ(verticals.size(), 5);

    auto box = polygon.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, -10.0);
    EXPECT_DOUBLE_EQ(box.min_y, -10.0);
    EXPECT_DOUBLE_EQ(box.max_x, 10.0);
    EXPECT_DOUBLE_EQ(box.max_y, 10.0);
}

TEST(PolygonTest, BoundBoxHeightAndCenterWorkCorrectly)
{
    Polygon polygon{{
        {0.0, 0.0},
        {4.0, 0.0},
        {4.0, 3.0},
        {0.0, 3.0}
    }};

    auto box = polygon.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 0.0);
    EXPECT_DOUBLE_EQ(box.min_y, 0.0);
    EXPECT_DOUBLE_EQ(box.max_x, 4.0);
    EXPECT_DOUBLE_EQ(box.max_y, 3.0);

    EXPECT_DOUBLE_EQ(polygon.Height(), 3.0);

    auto center = polygon.Center();
    EXPECT_DOUBLE_EQ(center.x, 2.0);
    EXPECT_DOUBLE_EQ(center.y, 1.5);
}

// TEST(FormatterTest, FormatsPointVectorInOneLine)
// {
//     std::vector<Point2D> points{{1.0, 2.0}, {3.0, 4.0}};

//     auto text = std::format("{}", points);

//     EXPECT_NE(text.find("(1.00, 2.00)"), std::string::npos);
//     EXPECT_NE(text.find("(3.00, 4.00)"), std::string::npos);
// }
