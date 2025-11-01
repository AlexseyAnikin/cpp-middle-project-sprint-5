#pragma once
#include "geometry.hpp"
#include <utility>
#include <vector>

namespace geometry::utils {

std::vector<Shape> ParseShapes(std::string_view input);

std::vector<std::pair<Shape, Shape>> FindAllCollisions(ReplaceMe shapes);

std::optional<size_t> FindHighestShape(ReplaceMe shapes);

}  // namespace geometry::utils