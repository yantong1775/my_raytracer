#pragma once

#include "../util.h"
#include "../core/ray.h"

class aabb
{
public:
    dvec3 m_min;
    dvec3 m_max;

public:
    aabb() : m_min(dvec3(INF)), m_max(dvec3(-INF)) {};
    aabb(const dvec3& min, const dvec3& max) : m_min(min), m_max(max) {};

    dvec3 min() const { return m_min; }
    dvec3 max() const { return m_max; }

    bool hit(const ray& r, double t_min, double t_max) const;
    
    // Expand this aabb to include another aabb.
    void expand(const aabb& bbox) 
    {
        m_min.x = std::min(m_min.x, bbox.m_min.x);
        m_min.y = std::min(m_min.y, bbox.m_min.y);
        m_min.z = std::min(m_min.z, bbox.m_min.z);
        m_max.x = std::max(m_max.x, bbox.m_max.x);
        m_max.y = std::max(m_max.y, bbox.m_max.y);
        m_max.z = std::max(m_max.z, bbox.m_max.z);
    }

    // Expand this aabb to include another point.
    void expand(const dvec3& p) 
    {
        m_min.x = std::min(m_min.x, p.x);
        m_min.y = std::min(m_min.y, p.y);
        m_min.z = std::min(m_min.z, p.z);
        m_max.x = std::max(m_max.x, p.x);
        m_max.y = std::max(m_max.y, p.y);
        m_max.z = std::max(m_max.z, p.z);
    }

    bool empty() const 
    {
        return m_min.x > m_max.x || m_min.y > m_max.y || m_min.z > m_max.z;
    }

    double surface_area() const 
    {
        if (empty()) return 0.0;
        auto extent = m_max - m_min;
        return 2 * (extent.x * extent.z +
                    extent.x * extent.y +
                    extent.y * extent.z);
    }

    dvec3 centroid() const
    {
        return (m_min + m_max) / 2.0;
    }

};
