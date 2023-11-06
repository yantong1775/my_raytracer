#pragma once

#include "material.h"

class Dielectric: public Material
{
public:
    double m_index;
public:
    Dielectric(const double idx) : m_index(idx)  {}

    virtual bool scatter(
        const dvec3& w_in, const hit_record& rec, dvec3& f_r, dvec3& w_out, double& pdf 
    ) const override 
    {
        // Assume light from air to dielectric or dielectric to air
        // Assume Refractive Index for air is 1.0
        f_r = Color(1.0, 1.0, 1.0);
        double ratio = rec.m_front_face ? (1.0/m_index) : m_index;
        dvec3 unit_w_in = normalize(w_in);

        double cos_theta = fmin(dot(unit_w_in, rec.m_normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        dvec3 scatter_dir;


        if (ratio * sin_theta > 1.0 || reflectance(cos_theta, ratio) > random_double()) 
            scatter_dir = reflect(unit_w_in, rec.m_normal);
        else scatter_dir = refract(unit_w_in, rec.m_normal, ratio);
                
        w_out = scatter_dir;
        return true;
    }

private:
    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1.-ref_idx) / (1. + ref_idx);
        r0 = r0 * r0;
        return r0 + (1-r0)*pow((1-cosine), 5);
    }
};