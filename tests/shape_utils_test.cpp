#include <gtest/gtest.h>

#include "geometry.hpp"
#include "shape_utils.hpp"

#include <variant>
#include <vector>

using namespace geometry;

TEST(ShapeUtilsTest, ParseShapesCreatesCircle) 
{
    auto shapes = utils::ParseShapes("circle 1 2 3");

    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<Circle>(shapes[0]));

    const auto& circle = std::get<Circle>(shapes[0]);
    EXPECT_DOUBLE_EQ(circle.center_p.x, 1.0);
    EXPECT_DOUBLE_EQ(circle.center_p.y, 2.0);
    EXPECT_DOUBLE_EQ(circle.radius, 3.0);
}

TEST(ShapeUtilsTest, ParseShapesCreatesLine) 
{
    auto shapes = utils::ParseShapes("line 1 2 3 4");

    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<Line>(shapes[0]));
}

TEST(ShapeUtilsTest, ParseShapesCreatesTriangle) 
{
    auto shapes = utils::ParseShapes("triangle 0 0 4 0 0 3");

    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<Triangle>(shapes[0]));
}

TEST(ShapeUtilsTest, ParseShapesSkipsInvalidShapes) 
{
    auto shapes = utils::ParseShapes(
        "circle 0 0 1; "
        "badshape 1 2 3; "
        "circle 0 0 -5; "
        "line 1 2 3 4"
    );

    ASSERT_EQ(shapes.size(), 2);
    EXPECT_TRUE(std::holds_alternative<Circle>(shapes[0]));
    EXPECT_TRUE(std::holds_alternative<Line>(shapes[1]));
}

TEST(ShapeUtilsTest, FindHighestShapeReturnsIndexOfHighestShape) 
{
    std::vector<Shape> shapes{
        Circle{{0.0, 0.0}, 1.0},
        Line{{0.0, 0.0}, {0.0, 10.0}},
        Circle{{0.0, 0.0}, 3.0}
    };

    auto index = utils::FindHighestShape(shapes);

    ASSERT_TRUE(index.has_value());
    EXPECT_EQ(*index, 1u);
}

TEST(ShapeUtilsTest, FindHighestShapeReturnsNulloptForEmptyInput) 
{
    std::vector<Shape> shapes;

    auto index = utils::FindHighestShape(shapes);

    EXPECT_FALSE(index.has_value());
}

TEST(ShapeUtilsTest, FindAllCollisionsFindsOverlappingBoundingBoxes) 
{
    std::vector<Shape> shapes{
        Circle{{0.0, 0.0}, 5.0},
        Circle{{3.0, 0.0}, 5.0},
        Circle{{100.0, 100.0}, 1.0}
    };

    auto collisions = utils::FindAllCollisions(shapes);

    ASSERT_EQ(collisions.size(), 1);
}