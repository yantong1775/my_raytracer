#pragma once

#include "material.h"

class Mirror: public Material
{
public:
    dvec3 m_albedo;
public:
    Mirror(const dvec3& a) : m_albedo(a) {}

    virtual bool scatter(
        const dvec3& w_in, const hit_record& rec, dvec3& f_r, dvec3& w_out, double& pdf 
    ) const override
    {
        w_out = reflect(w_in, rec.m_normal);
        
        f_r = m_albedo;
        return dot(w_out, rec.m_normal) > 0.0f;
    }
};