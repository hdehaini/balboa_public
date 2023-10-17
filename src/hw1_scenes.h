#pragma once

#include "balboa.h"
#include "vector.h"
#include "matrix.h"
#include <variant>
#include <vector>

namespace hw1 {

struct Circle {
    Vector2 center;
    Real radius;
    Vector3 color;
    Real alpha;
    Matrix3x3 transform;
};

struct CircleScene {
    Vector2i resolution;
    Vector3 background;
    std::vector<Circle> objects;
};

extern CircleScene hw1_2_scenes[];

struct Rectangle {
    Vector2 p_min, p_max;
    Vector3 color;
    Real alpha;
    Matrix3x3 transform;
};

struct Triangle {
    Vector2 p0, p1, p2;
    Vector3 color;
    Real alpha;
    Matrix3x3 transform;
};

// Define a structure for representing a 2D line segment
struct Line {
    Vector2 start;    // Starting point of the line
    Vector2 end;      // Ending point of the line
    Real width;       // Line width
    Vector3 color;    // Line color
    Real alpha;       // Line transparency
    Matrix3x3 transform; // Transformation matrix
};

// Define a structure for representing a quadratic Bézier curve
struct Quadratic {
    Vector2 start;   // Starting point of the curve
    Vector2 control; // Control point
    Vector2 end;     // Ending point of the curve
    Real width;      // Curve width
    Vector3 color;   // Curve color
    Real alpha;      // Curve transparency
    Matrix3x3 transform; // Transformation matrix
};

// Define a structure for representing a cubic Bézier curve
struct Cubic {
    Vector2 start;      // Starting point of the curve
    Vector2 control1;   // First control point
    Vector2 control2;   // Second control point
    Vector2 end;        // Ending point of the curve
    Real width;         // Curve width
    Vector3 color;      // Curve color
    Real alpha;         // Curve transparency
    Matrix3x3 transform; // Transformation matrix
};

using Shape = std::variant<Circle, Rectangle, Triangle, Line, Quadratic, Cubic>;

inline void set_color(Shape &shape, const Vector3 &color) {
    std::visit([&](auto &s) { s.color = color; }, shape);
}
inline Vector3 get_color(const Shape &shape) {
    return std::visit([](const auto &s) { return s.color; }, shape);
}
inline void set_transform(Shape &shape, const Matrix3x3 &transform) {
    std::visit([&](auto &s) { s.transform = transform; }, shape);
}
inline Matrix3x3 get_transform(const Shape &shape) {
    return std::visit([](const auto &s) { return s.transform; }, shape);
}

struct Scene {
    Vector2i resolution;
    Vector3 background;
    std::vector<Shape> shapes;
};

Scene parse_scene(const fs::path &filename);

std::ostream& operator<<(std::ostream &os, const Shape &shape);
std::ostream& operator<<(std::ostream &os, const Scene &scene);

} // namespace hw1
