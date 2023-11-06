#pragma once

#include "../util.h"

class SceneLight {
 public:
  virtual dvec3 sample_L(const dvec3 p, dvec3& wi,
                            double& distToLight, double& pdf) const = 0;
  
  virtual bool is_delta_light() const = 0;

};