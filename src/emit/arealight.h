#pragma once

#include "scenelight.h"

class AreaLight : public SceneLight 
{
 public:
  AreaLight(const dvec3 rad, 
            const dvec3 pos,   const dvec3 dir, 
            const dvec3 dim_x, const dvec3 dim_y);
 
  dvec3 sample_L(const dvec3 p, dvec3& wi, double& distToLight,
                    double& pdf) const;
  bool is_delta_light() const { return false; }

  dvec3 radiance;
  dvec3 position;
  dvec3 direction;
  dvec3 dim_x;
  dvec3 dim_y;
  double area;

}; // class AreaLight