#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include <cmath>
#include <ctype.h>

#include "curves3d.h"
#include "timer.h"

static const size_t DEFAULT_NUMBER_OF_CURVES = 10000;

static const double MAX_CIRCLE_RADIUS = 10.;
static const double MAX_SEMIAXIS_ELLIPSE_RADIUS = 10.;
static const double MAX_HELIX_RADIUS = 10.;
static const double ABS_MAX_HELIX_STEP = 10.;

static const double PI = std::acos(-1);
static const double T_IN_TASK = PI / 4;

using Curve3dShPtr = std::shared_ptr<curves3d::Curve3D>;

std::vector<Curve3dShPtr> curves3d_rand_generate(size_t n) {
    std::vector<Curve3dShPtr> curves_vec;
    curves_vec.reserve(n);

    curves3d::RandomCurve3dGenerator curve_generator;
    curve_generator.set_max_radius_circle(MAX_CIRCLE_RADIUS)
                 .set_max_radius_semiaxis_ellipse(MAX_SEMIAXIS_ELLIPSE_RADIUS)
                 .set_max_radius_helix(MAX_HELIX_RADIUS)
                 .set_max_step_helix(ABS_MAX_HELIX_STEP);

    for(int i = 0; i < n; ++i) {
        curves_vec.emplace_back(curve_generator.get_random_curve3d());
    }
    return curves_vec;
}


bool cstr_is_digit(char* cstr) {
    while(*cstr != '\0') {
        if(!isdigit(*cstr)) return false;
        ++cstr;
    }
    return true;
}


int main(int argc, char* argv[]) {
    std::vector<Curve3dShPtr> curves_ptr_vec;

    if(argc == 1) {
        curves_ptr_vec = curves3d_rand_generate(DEFAULT_NUMBER_OF_CURVES);
    } else if(cstr_is_digit(argv[1])) {
        curves_ptr_vec = curves3d_rand_generate(atoi(argv[1]));
        }
        else {
            std::cout << "Invalid argument. Argument must be digit and over zero" << "\n";
            return 0;
        }

    for(const auto& curve_ptr : curves_ptr_vec) {
        std::cout << curve_ptr -> get_type() << " Point(PI/4): " << curve_ptr -> get_point(T_IN_TASK);
        std::cout << " Derivative(PI/4): " <<  curve_ptr -> get_first_derivative(T_IN_TASK) << "\n";
    }

    std::vector<std::shared_ptr<curves3d::Circle>> circles_ptrs;
    for(auto& curve_ptr : curves_ptr_vec) {
        if(curve_ptr -> get_type() == curves3d::CURVE3D_TYPE::CIRCLE) {
            circles_ptrs.push_back(std::static_pointer_cast<curves3d::Circle>(curve_ptr));
        }
    }

    size_t n = circles_ptrs.size();
    std::cout << '\n' << "Size of container with circles: " << n << '\n';

    std::sort(circles_ptrs.begin(), circles_ptrs.end(), [](std::shared_ptr<curves3d::Circle> l,
                                                           std::shared_ptr<curves3d::Circle> r){
        return l -> get_radius() < r -> get_radius();
    });

    std::cout << '\n' << "Container with circles is sorted." << '\n';

    double radiuses_sum = 0.;
    int i;

    {
        Timer timer("Computation time of sum raduises circles with OpenMP:");
    #pragma for shared(circles_ptrs, n) reduction(+:radiuses_sum)
        for(i = 0; i < n; ++i) {
            radiuses_sum += circles_ptrs[i] -> get_radius();
        }

        std::cout << '\n' << "Sum of radiuses with OpenMP: " << radiuses_sum << std::endl;
    }

    {
        radiuses_sum = 0.;
        Timer timer("Computation time of sum raduises circles without OpenMP:");
        for(const auto& circle_ptr : circles_ptrs) {
            radiuses_sum += circle_ptr -> get_radius();
        }
        std::cout << '\n' << "Sum of radiuses without OpenMP : " << radiuses_sum << std::endl;
    }

    if(argc == 1) {
        std::cout << '\n' << "You can use programm with argument. Example: \"./test n\", "\
                    "where n is count of generate curves" << "\n";
    }
    return 0;
}
