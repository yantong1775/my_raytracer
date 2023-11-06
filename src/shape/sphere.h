#pragma once

#include "../core/hittable.h"

class sphere : public hittable
{
public:
    dvec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_mat_ptr;
public:
    sphere() {}
    sphere(dvec3 cen, double r, std::shared_ptr<Material> m) 
    : m_center(cen), m_radius(r), m_mat_ptr(m) {};

    virtual bool hit(const ray& r, double t_min, double t_max, 
            hit_record& rec) const override
    {
        dvec3 oc = r.origin() - m_center;
        auto a = dot(r.direction(), r.direction());
        auto hb = dot(oc , r.direction());
        auto c = dot(oc, oc) - m_radius*m_radius;
        auto discriminant = hb*hb - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-hb - sqrtd) / a;
        if (root < t_min || root > t_max) {
            root = (-hb + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        rec.m_t = root;
        rec.m_p = r.at(rec.m_t);
        dvec3 out_normal = (rec.m_p - m_center) / m_radius;
        rec.set_face_normal(r, out_normal);
        get_uv(out_normal, rec.m_u, rec.m_v);
        rec.m_mat_ptr = m_mat_ptr;

        return true;
    }
    
    virtual bool bounding_box(double time0, double time1, 
            aabb& output_box) const override
    {
        output_box = aabb(
            m_center - dvec3(m_radius),
            m_center + dvec3(m_radius)
        );
        return true;
    }

private:
    static void get_uv(dvec3 pos, double& u, double& v)
    {
        auto theta = acos(-pos.y);
        auto phi = atan2(-pos.z, pos.x) + PI;
        u = phi / (2*PI);
        v = theta / PI;
    }
};
