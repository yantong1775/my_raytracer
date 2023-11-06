#pragma once

#include "material.h"
#include "../textures/texture.h"
#include "../textures/solid_color.h"

class Lambertian: public Material
{
public:
    shared_ptr<texture> m_albedo;
public:
    Lambertian(const Color& a) : m_albedo(make_shared<solid_color>(a)) {}
    Lambertian(shared_ptr<texture> a) : m_albedo(a) {}

    virtual dvec3 fr(const dvec3& w_in, const dvec3& w_out, const hit_record& rec) const override
    {
        return m_albedo->value(rec.m_u, rec.m_v, rec.m_p) * invPI;
    }

    virtual bool scatter(
        const dvec3& w_in, const hit_record& rec, dvec3& f_r, dvec3& w_out, double& pdf 
    ) const override
    {
        w_out = cosine_sample_hemisphere(pdf);
        f_r = fr(w_in, w_out, rec);
        return true;
    }
};