#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
// #include <print>
#include <iostream>
#include <ranges> 
#include <stdexcept>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) 
{
    for(const auto& other : others)
    {
        try
        {
            auto point = intersections::GetIntersectPoint(shape, other);

            if(point.has_value())
            {
                std::cout << "intersection point:" << "(" << point->x << ", " << point->y << ")";
                // std::println("intersection point: ({}, {})", point->x, point->y);
            }
            else
            {
                std::cout << "Shapes do not intersect";
                // std::println("Shapes do not intersect");
            }
        }
        catch (const std::logic_error&)
        {
            std::cout << "Intersection is not supported for these shapes";
            // std::println("Intersection is not supported for these shapes");
        }
    }
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes)
{   
    auto selected = shapes | views::take(5);

    for(const auto& shape : selected)
    {
        auto distance = queries::DistanceToPoint(shape, p);

        std::cout << "Distance fron point (" << p.x << ", " << p.y << ") to shape = " << distance;
        // std::println("Distance fron point ({}, {}) to shape = {}", p.x, p.y, distance);
    }
}

void PerformShapeAnalysis(std::span<const Shape> shapes) 
{
    auto collisions = utils::FindAllCollisions(shapes);

    for(const auto& [lhs, rhs] : collisions)
    {
        std::cout << "Collision detected";
        // std::println("Collision detected");
    }

    auto highest = utils::FindHighestShape(shapes);

    if(highest.has_value())
    {
        std::cout << "Highest shape index = " << *highest << ", height = " <<  queries::GetHeight(shapes[*highest]);
        // std::println("Highest shape index = {}, height = {}", *highest, 
        //     queries::GetHeight(shapes[*highest]));
    }

    for(auto i : views::iota(size_t{0}, shapes.size()))
    {
        for(auto j : views::iota(i + 1, shapes.size()))
        {
            auto distance = queries::DistanceBetweenShapes(shapes[i], shapes[j]);

            if(distance.has_value())
            {
                std::cout << "Distance between shapes " << i << " and " << j << " = " << *distance;
                // std::println("Distance between shapes {} and {} = {}", i, j, *distance);
            }

        }
    }

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Найти все пересечения между фигурами используя метод Bounding Box
     *     - Найти самую высокую фигуру (чья высота наибольшая)expected
     *     - Вывести расстояние между любыми двумя фигурами, которые поддерживают данную функциональность
     */
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) 
{
    auto high_shapes = shapes | views::filter([](const Shape& shape)
    {
        return queries::GetHeight(shape) > 50.0;
    })
    | views::take(3);

    for(const auto& shape : high_shapes)
    {
        std::cout << "High shape height = " << queries::GetHeight(shape);
        // std::println("High shape height = {}", queries::GetHeight(shape));
    }

    auto minmax = std::ranges::minmax_element(shapes, {}, [](const Shape& shape)
    {
        return queries::GetHeight(shape);
    });

    if(minmax.min != shapes.end())
    {
        // std::println("Min height = {}", queries::GetHeight(*minmax.min));
        // std::println("Max height = {}", queries::GetHeight(*minmax.max));
        std::cout << "Mix height = " << queries::GetHeight(*minmax.min);
        std::cout << "Max height = " << queries::GetHeight(*minmax.max);
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    // std::println("Parsed {} shapes", shapes.size());
    std::cout << "Parsed " << shapes.size() << "shapes";

    // Выведите индекс каждой фигуры и её высоту
    for (const auto& [index, shape] : views::enumerate(shapes))
    {
        std::cout << "Shape #" << index << "height = " << queries::GetHeight(shape);
        // std::println("Shape #{} height = {}", index, queries::GetHeight(shape));
    }

    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;

    for(const auto& shape : shapes)
    {
        std::visit([&](const auto& concrete_shape)
        {
            auto lines = concrete_shape.Lines();

            for(std::size_t i = 0; i < lines.x.size(); ++i)
            {
                points.push_back({lines.x[i], lines.y[i]});
            }
        },
        shape
    );
    }

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема 
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    auto null_points = convex_hull::GrahamScan(points);

    if(!null_points.empty())
    {
        shapes.push_back(Polygon{null_points});
        geometry::visualization::Draw(shapes);
    }

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        auto triangles = triangulation::DelaunayTriangulation(points);

        std::cout << "Delaunay triangels count = " << triangles.size();
        // std::println("Delaunay triangels count = {}", triangles.size());

        geometry::visualization::Draw(triangles);
        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //
    }
    return 0;
}