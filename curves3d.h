#pragma once

#include <iostream>
#include <random>

namespace curves3d{
struct Point3D {
    double x;
    double y;
    double z;
};

std::ostream& operator<< (std::ostream& os, Point3D point);

struct Vector3D {
    double x;
    double y;
    double z;
};

std::ostream& operator<< (std::ostream& os, Vector3D vec);

enum CURVE3D_TYPE{
    ELLIPSE = 0,
    CIRCLE = 1,
    HELIX = 2,
};

std::ostream& operator<< (std::ostream& os, CURVE3D_TYPE type);

static const short SIZE_CURVE3D_TYPE = 3;

struct Curve3D {
    virtual Point3D get_point(double t) const = 0;
    virtual Vector3D get_first_derivative(double t) const = 0;
    virtual CURVE3D_TYPE get_type() const = 0;
    virtual ~Curve3D() = default;
};

//Хотя круг и является частным случаем эллипса, но для решения
//задания наследовать класс Circle от класса Ellipse смысла нет
struct Circle : public Curve3D {
    Circle(double r);
    Point3D get_point(double t) const override;
    Vector3D get_first_derivative(double t) const override;
    CURVE3D_TYPE get_type() const override;
    double get_radius() const;

private:
    double radius;
};

struct Ellipse : public Curve3D {
    Ellipse(double x, double y);
    Point3D get_point(double t) const override;
    Vector3D get_first_derivative(double t) const override;
    CURVE3D_TYPE get_type() const override;

private:
    double radius_major_semiaxis;
    double radius_minor_semiaxis;
};

struct Helix : public Curve3D {
    Helix(double r, double s);
    Point3D get_point(double t) const override;
    Vector3D get_first_derivative(double t) const override;
    CURVE3D_TYPE get_type() const override;

private:
    double radius;
    double step;
};

struct RandomCurve3dGenerator {
    RandomCurve3dGenerator();
    Curve3D* get_random_curve3d();
    RandomCurve3dGenerator& set_max_radius_circle(double val);
    RandomCurve3dGenerator& set_max_radius_semiaxis_ellipse(double val);
    RandomCurve3dGenerator& set_max_radius_helix(double val);
    RandomCurve3dGenerator& set_max_step_helix(double val);

private:
    double max_radius_circle_ = 10.;
    double max_radius_semiaxis_ellipse_ = 10.;
    double max_radius_helix_ = 10.;
    double max_step_helix_ = 1.;

    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_int_distribution<> dist_type{0, SIZE_CURVE3D_TYPE - 1};

    double random_radius(double max_val);
};
}
