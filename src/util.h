#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <iostream>

#include "glm/glm.hpp"

using glm::dvec3;
using glm::dvec2;
using glm::normalize;
using glm::dot;
using glm::transpose;
using glm::dmat3;

using std::shared_ptr;
using std::make_shared;

#define Color glm::dvec3

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;
const double inv2PI = 1. / (2. * PI);
const double invPI = 1. / PI;
const double inv4PI = 1. / (4. * PI);

// CAUTIOUS: inline function should be defined in header file.
// If you do not define a inline function, don't implement the function in header file.
// Otherwise, it will be defined in every cpp file that includes the header file.
inline double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Can't using <random>'s distribution.
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) 
{
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max+1));
}


inline dvec3 random_vec3()
{
    return dvec3(random_double(), random_double(), random_double());
}   

inline dvec3 random_vec3(double min, double max)
{
    return dvec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline dvec3 random_in_unit_sphere()
{
    while (true)
    {
        dvec3 p = random_vec3(-1., 1.);
        if (dot(p, p) >= 1.) continue;
        return p;
    }
}

inline dvec3 random_in_hemisphere(const dvec3& normal)
{
    dvec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline dvec3 random_in_unit_disk()
{
    while (true)
    {
        dvec3 p = dvec3(random_double(-1., 1.), random_double(-1., 1.), 0.);
        if (dot(p, p) >= 1.) continue;
        return p;
    }
}
inline dvec3 random_unit_vector()
{
    return glm::normalize(random_in_unit_sphere());
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline bool near_zero(dvec3 v)
{
    const auto s = 1e-8;
    return (fabs(v[0]) < s && fabs(v[1]) < s && fabs(v[2]) < s);
}

inline dvec3 reflect(const dvec3& v, const dvec3& n)
{
    return v - 2*dot(v, n)*n;
}

inline dvec3 refract(const dvec3& v, const dvec3& n, double ratio)
{
    double cos_theta = fmin(dot(v, n), 1.0);
    
    dvec3 r_out_perp = ratio*(v - cos_theta*n);
    dvec3 r_out_parallel = -sqrt(fabs(1.0 - dot(r_out_perp, r_out_perp)))*n;
    
    return r_out_perp + r_out_parallel;
}


dvec3 uniform_sample_hemisphere(double& pdf);
dvec3 cosine_sample_hemisphere(double& pdf);
dvec3 random_cosine_direction(double& pdf);

dvec2 concentric_sample_disk();

class Image
{
private:
    int m_width, m_height;
    Color *m_data;
public:
    Image(int width, int height);
    ~Image();
    void setPixel(int x, int y, Color c);
    void write(const char *filename, int spp);
};

unsigned char* load(const char *filename, int bpp, int& width, int& height);

std::ostream &operator<<(std::ostream &os, dvec3 const &a);

