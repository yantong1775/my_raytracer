#pragma once

#include "../core/hittable.h"
#include "../material/material.h"
#include "../material/isotropic.h"
#include "../textures/texture.h"

class constant_medium : public hittable
{
public:
    shared_ptr<hittable> m_boundary;
    shared_ptr<Material> m_phase_function;

    double neg_inv_density;

public:
    constant_medium() {};

    constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
        : m_boundary(b), m_phase_function(make_shared<isotropic>(a)), neg_inv_density(-1/d) {}
    constant_medium(shared_ptr<hittable> b, double d, Color a)
        : m_boundary(b), m_phase_function(make_shared<isotropic>(a)), neg_inv_density(-1/d) {}

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        hit_record rec1, rec2;

        if (!m_boundary->hit(r, -INF, INF, rec1)) return false;
        if (!m_boundary->hit(r, rec1.m_t + 0.001, INF, rec2)) return false;

        if (rec1.m_t < t_min) rec1.m_t = t_min;
        if (rec2.m_t > t_max) rec2.m_t = t_max;

        if (rec1.m_t >= rec2.m_t) return false;

        if (rec1.m_t < 0) rec1.m_t = 0;

        const double ray_length = sqrt(dot(r.m_dir, r.m_dir));
        const double distance_inside_boundary = (rec2.m_t - rec1.m_t) * ray_length;
        const double hit_distance = neg_inv_density * log(random_double());

        if (hit_distance > distance_inside_boundary) return false;


        rec.m_t = rec1.m_t + hit_distance / ray_length;
        rec.m_p = r.at(rec.m_t);

        rec.m_normal = {0, 0, 1};   // arbitrary
        rec.m_front_face = true;     // also arbitrary
        rec.m_mat_ptr = m_phase_function;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override 
    {
        return m_boundary->bounding_box(time0, time1, output_box);
    }
    
};