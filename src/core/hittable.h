#pragma once

#include "../acceleration/aabb.h"

class Material;

struct hit_record
{
    dvec3 m_p; // hit point 
    dvec3 m_normal; // normal vector
    std::shared_ptr<Material> m_mat_ptr; // material pointer
    double m_t; // time
    double m_u, m_v; // texture coordinates
    bool m_front_face;

    inline void set_face_normal(const ray& r, const dvec3& outward_normal)
    {
        m_front_face = dot(r.direction(), outward_normal) < 0;
        m_normal = m_front_face ? outward_normal : -outward_normal;
    }
};

class hittable 
{
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
        virtual double pdf_value(const dvec3& o, const dvec3& v) const 
        {
            return 0.0;
        }

        virtual dvec3 random(const dvec3& o) const 
        {
            return dvec3(1, 0, 0);
        }
};