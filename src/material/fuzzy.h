#pragma once

#include "material.h"

class Fuzzy: public Material
{
public:
    dvec3 m_albedo;
    double m_fuzz;
public:
    Fuzzy(const dvec3& a, const double f) : m_albedo(a), m_fuzz(f) {}

    virtual bool scatter(
        const dvec3& w_in, const hit_record& rec, dvec3& f_r, dvec3& w_out, double& pdf 
    ) const override 
    {
        auto reflect_dir = reflect(w_in, rec.m_normal);
        auto scatter_dir = reflect_dir + m_fuzz*random_in_unit_sphere();
        
        w_out = scatter_dir;
        f_r = m_albedo;
        return dot(scatter_dir, rec.m_normal) > 0.0f;
    }
};