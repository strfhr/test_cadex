#include <cmath>
#include <iostream>
#include <exception>

#include "curves3d.h"
namespace curves3d{

std::ostream& operator<< (std::ostream& os, Point3D point) {
    return os << "{" << point.x << "," << point.y << "," << point.z << "}";
}


std::ostream& operator<< (std::ostream& os, Vector3D vec) {
    return os << "{" << vec.x << "," << vec.y << "," << vec.z << "}";
}


std::ostream& operator<< (std::ostream& os, CURVE3D_TYPE type) {
    switch(type) {
        case CURVE3D_TYPE::ELLIPSE:
            os << "Ellipse";
            break;
        case CURVE3D_TYPE::CIRCLE:
            os << "Circle";
            break;
        case CURVE3D_TYPE::HELIX:
            os << "Helix";
            break;
    }
    return os;
}


Circle::Circle(double r) {
    if(r <= 0) throw std::invalid_argument("Radius must be positive");
    radius = r;
}


Point3D Circle::get_point(double t) const {
    return Point3D{std::cos(t) * radius, std::sin(t) * radius, 0};
}


Vector3D Circle::get_first_derivative(double t) const {
    return Vector3D{std::sin(t) * -1 * radius, std::cos(t) * radius, 0};
}


CURVE3D_TYPE Circle::get_type() const {
    return CURVE3D_TYPE::CIRCLE;
}


double Circle::get_radius() const {
    return radius;
}


Ellipse::Ellipse(double x, double y) {
    if(x <= 0 || y <= 0  ) throw std::invalid_argument("All radiuses must be positive");
    radius_major_semiaxis = x;
    radius_minor_semiaxis = y;
}


Point3D Ellipse::get_point(double t) const {
    return Point3D{std::cos(t) * radius_major_semiaxis, std::sin(t) * radius_minor_semiaxis, 0};
}


Vector3D Ellipse::get_first_derivative(double t) const {
    return Vector3D{std::sin(t) * -1 * radius_major_semiaxis, std::cos(t) * radius_minor_semiaxis, 0};
}


CURVE3D_TYPE Ellipse::get_type() const {
    return CURVE3D_TYPE::ELLIPSE;
}


Helix::Helix(double r, double s) {
    if(r <= 0) throw std::invalid_argument("Radius must be positive");
    radius = r;
    step = s;
}


Point3D Helix::get_point(double t) const {
    return Point3D{std::cos(t) * radius, std::sin(t) * radius, step * t};
}


Vector3D Helix::get_first_derivative(double t) const {
    return Vector3D{std::sin(t) * -1 * radius, std::cos(t) * radius, step};
}


CURVE3D_TYPE Helix::get_type() const {
    return CURVE3D_TYPE::HELIX;
}


RandomCurve3dGenerator::RandomCurve3dGenerator()
    : gen_(rd_()){
}

Curve3D* RandomCurve3dGenerator::get_random_curve3d() {
    int curve_type = dist_type(gen_);
    switch(static_cast<curves3d::CURVE3D_TYPE>(curve_type)) {
        case curves3d::CURVE3D_TYPE::CIRCLE :
            return new Circle(random_radius(max_radius_circle_));
        case curves3d::CURVE3D_TYPE::ELLIPSE :
            return new Ellipse(random_radius(max_radius_semiaxis_ellipse_),
                                random_radius(max_radius_semiaxis_ellipse_));
        case curves3d::CURVE3D_TYPE::HELIX :
            std::uniform_real_distribution<> dist_step(-max_step_helix_, max_step_helix_);
            double step = dist_step(gen_);
            return new Helix(random_radius(random_radius(max_radius_helix_)), step);
    }
    return nullptr;
}


RandomCurve3dGenerator& RandomCurve3dGenerator::set_max_radius_circle(double val) {
    if(!(val > 0)) {
        throw std::invalid_argument("Radius must be positive");
    }
    max_radius_circle_ = val;
    return *this;
}


RandomCurve3dGenerator& RandomCurve3dGenerator::set_max_radius_semiaxis_ellipse(double val) {
    if(!(val > 0)) {
        throw std::invalid_argument("Radius must be positive");
    }
    max_radius_semiaxis_ellipse_ = val;
    return *this;
}


RandomCurve3dGenerator& RandomCurve3dGenerator::set_max_radius_helix(double val) {
    if(!(val > 0)) {
        throw std::invalid_argument("Radius must be positive");
    }
    max_radius_helix_ = val;
    return *this;
}


RandomCurve3dGenerator& RandomCurve3dGenerator::set_max_step_helix(double val) {
    max_step_helix_ = val;
    return *this;
}


double RandomCurve3dGenerator::random_radius(double max_val) {
    std::uniform_real_distribution<> dist_rad(0., max_val);
    double r = dist_rad(gen_);
    while(!(r > 0)) {
        r = dist_rad(gen_);
    }
    return r;
}
}
