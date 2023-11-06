#pragma once

#include "../core/hittable.h"
#include "../core/hittable_list.h"

#include "aarect.h"

class box : public hittable
{
public:
    dvec3 m_min, m_max;
    hittable_list m_sides;

public:
    box() {}
    box(const dvec3 min, const dvec3 max, shared_ptr<Material> mat_ptr)
        : m_min(min), m_max(max)
    {
        m_sides.add(make_shared<xy_rect>(min.x, max.x, min.y, max.y, max.z, mat_ptr));
        m_sides.add(make_shared<xy_rect>(min.x, max.x, min.y, max.y, min.z, mat_ptr));

        m_sides.add(make_shared<xz_rect>(min.x, max.x, min.z, max.z, max.y, mat_ptr));
        m_sides.add(make_shared<xz_rect>(min.x, max.x, min.z, max.z, min.y, mat_ptr));

        m_sides.add(make_shared<yz_rect>(min.y, max.y, min.z, max.z, max.x, mat_ptr));
        m_sides.add(make_shared<yz_rect>(min.y, max.y, min.z, max.z, min.x, mat_ptr));
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
    {
        return m_sides.hit(r, t_min, t_max, rec);
    }
    
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const
    {
        output_box = aabb(m_min, m_max);
        return true;
    }
};