#pragma once

#include "../util.h"
#include "../core/hittable.h"

class Material
{
public:
    virtual bool scatter
    (const dvec3& w_in, const hit_record& rec, dvec3& f_r, dvec3& w_out, double& pdf
    ) const
    {
        return 0;
    }

    virtual dvec3 fr(const dvec3& w_in, const dvec3& w_out, const hit_record& rec) const
    {
        return dvec3(0, 0, 0);
    }

    virtual Color emitted(double u, double v, const dvec3& p) const
    {
        return Color(0, 0, 0);
    }
};
