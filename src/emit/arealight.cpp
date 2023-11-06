#include "arealight.h"

AreaLight::AreaLight(const dvec3 rad, 
                     const dvec3 pos,   const dvec3 dir, 
                     const dvec3 dim_x, const dvec3 dim_y)
  : radiance(rad), position(pos), direction(dir),
    dim_x(dim_x), dim_y(dim_y), area(glm::length(dim_x) * glm::length(dim_y)) { }
    

dvec3 AreaLight::sample_L(const dvec3 p, dvec3& wi, 
                             double& distToLight, double& pdf) const 
{

    dvec2 sample = dvec2(random_double(), random_double()) - dvec2(0.5f, 0.5f);
    dvec3 d = position + sample.x * dim_x + sample.y * dim_y - p;
    double cosTheta = dot(d, direction);
    double dist = glm::length(d);
    double sqDist = dist * dist;
    wi = d / dist;
    distToLight = dist;
    pdf = sqDist / (area * fabs(cosTheta));
    return cosTheta < 0 ? radiance : dvec3();
};
