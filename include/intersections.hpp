#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>

namespace geometry::intersections {

/*
* Класс для поиска пересечений между двумя фигурами
*
* Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
*    - Line   & Line
*    - Line   & Circle
*    - Circle & Circle
*
* Для всех остальных требуется выбросить исключение std::logic_error
*/
class IntersectionVisitor {
public:

    std::optional<Point2D> operator()(const Line& lhs, const Line& rhs) const 
    {
        const auto p = lhs.start;
        const auto r = lhs.end - lhs.start;

        const auto q = rhs.start;
        const auto s = rhs.end - rhs.start;

        const double r_cross_s = r.Cross(s);

        if(std::abs(r_cross_s) < 1e-10)
        {
            return std::nullopt;
        }

        const auto q_minus_p = q - p;
        const double t = q_minus_p.Cross(s) / r_cross_s;
        const double u = q_minus_p.Cross(r) / r_cross_s;

        if(t < 0.0 || t > 1.0 || u < 0.0 || u > 1.0)
        {
            return std::nullopt;
        }

        return Point2D{p.x + t * r.x, p.y + t * r.y};
    }

    std::optional<Point2D> operator()(const Line& line, const Circle& circle) const
    {
        const auto d = line.end - line.start;
        const auto f = line.start - circle.center_p;

        const auto a = d.Dot(d);
        const auto b = 2.0 * f.Dot(d);
        const auto c = f.Dot(f) - circle.radius * circle.radius;

        const double discriminant = b * b - 4.0 * a * c;

        if(discriminant < 0.0)
        {
            return std::nullopt;
        }

        const double sqrt_d = std::sqrt(discriminant);
        const double t1 = (-b - sqrt_d) / (2.0 * a);
        const double t2 = (-b + sqrt_d) / (2.0 * a);

        double t = -1.0;

        if(t1 >= 0.0 && t1 <= 1.0)
        {
            t = t1;
        }
        else if(t2 >= 0.0 && t2 <= 1.0)
        {
            t = t2;
        }

        if(t < 0.0)
        {
            return std::nullopt;
        }

        return Point2D(line.start.x + d.x * t, line.start.y + d.y * t);
    }

    std::optional<Point2D> operator()(const Circle& circle, const Line& line) const
    {
        return (*this)(line, circle);
    }

    std::optional<Point2D> operator()(const Circle& lhs, const Circle& rhs) const
    {
        const double d = lhs.center_p.DistanceTo(rhs.center_p);

        if(d > lhs.radius + rhs.radius)
        {
            return std::nullopt;
        }

        if(d < std::abs(lhs.radius - rhs.radius))
        {
            return std::nullopt;
        }

        if(d == 0.0 && lhs.radius == rhs.radius)
        {
            return std::nullopt;
        }

        const double a = (lhs.radius * lhs.radius - rhs.radius * rhs.radius + d * d) / (2.0 * d);
        const double h_sq = lhs.radius * lhs.radius - a * a;

        if(h_sq < 0.0)
        {
            return std::nullopt;
        }

        const double h = std::sqrt(h_sq);

        const Point2D p2{lhs.center_p.x + a *(rhs.center_p.x - lhs.center_p.x) / d,
                         lhs.center_p.y + a *(rhs.center_p.y - lhs.center_p.y) / d
                    };

        return Point2D{p2.x - h * (rhs.center_p.y - lhs.center_p.y) / d.
                       p2.y + h * (rhs.center_p.x - lhs.center_p.x) / d
                    };

    template <typename Lhs, typename Rhs>
    std::optional<Point2D> operator()(const Lhs&, const Rhs&) const
    {
        throw std::logic_error{"Intersection is not supported for these shapes"};
    }
};

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    return std::visit(IntersectionVisitor{}, shape1, shape2);
}

}  // namespace geometry::intersections