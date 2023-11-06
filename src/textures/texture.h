#pragma once

#include "../util.h"

class texture
{
public:
    virtual Color value(double u, double v, const dvec3 &p) const = 0;
};

