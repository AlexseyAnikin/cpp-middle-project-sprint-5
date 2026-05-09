#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <print>
#include <ranges>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) 
{
    auto supported = others | views::filter([&](const Shape& other) 
    {
        return queries::CanShapesIntersect(shape, other);
    });

    for(const auto& other : supported)
    {
        queries::FindIntersection(shape, other).transform([&](const auto& points)
        {
            if(!points.empty())
            {
                std::println("Intersection found between shapes");

                for(const auto& p : points)
                {
                    std::println("Point({}, {})", p.x, p.y);
                }
            }

            return points;
        }).or_else([&](const auto&) 
        {
            std::println("Shapes do not intersectt");

            return std::optional<std::vector<Point2D>>{};
        });
    }
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes)
{   
    auto selected = shapes | views::take(5);

    for(const auto& shape : selected)
    {
        auto distance = queries::DistanceToPoint(shape, p);

        std::println("Distance fron point ({}, {}) to shape = {}", p.x, p.y, distance);
    }
}

void PerformShapeAnalysis(std::span<const Shape> shapes) 
{
    auto collisions = utils::FindAllCollisions(shapes);

    for(const auto& [lhs, rhs] : collisions)
    {
        std::println("Collision detected");
    }

    auto highest = utils::FindHighestShape(shapes);

    if(highest.has_value())
    {
        std::println("Highest shape index = {}, height = {}", *highest, 
            queries::GetHeight(shapes[*highest]));
    }

    for(auto i : views::iota(size_t{0}, shapes.size()))
    {
        for(auto j : views::iota(i + 1, shapes.size()))
        {
            if(queries::CanMeasureDistance(shapes[i], shapes[j]))
            {
                auto distance = queries::DistanceBetweenShapes(shapes[i], shapes[j]);

                std::println("Distance between shapes {} and {} = {}", i, j, distance);
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
        std::println("High shape height = {}", queries::GetHeight(shape));
    }

    auto minmax = std::ranges::minmax_element(shapes, {}, [](const Shape& shape)
    {
        return queries::GetHeight(shape);
    });

    if(minmax.min != shapes.end())
    {
        std::println("Min height = {}", queries::GetHeight(*minmax.min));
        std::println("Max height = {}", queries::GetHeight(*minmax.max));
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту

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

    /* ваш код здесь */

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема 
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    /* ваш код здесь */

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

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