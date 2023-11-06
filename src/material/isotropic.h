#pragma once

#include "material.h"
#include "../textures/solid_color.h"
#include "../textures/texture.h"

class isotropic : public Material {
public:
    isotropic(Color c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const dvec3& w_in, const hit_record& rec, dvec3& f_r, dvec3& w_out, double& pdf 
    ) const override 
    {
        w_out = random_in_unit_sphere();
        f_r = albedo->value(rec.m_u, rec.m_v, rec.m_p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};