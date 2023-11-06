#pragma once
#include <glm/glm.hpp>

using namespace glm;

class ray
{
public:
    dvec3 m_orig;
    dvec3 m_dir;

public:
    ray() {};
    ray(const dvec3& origin, const dvec3& direction)
        : m_orig(origin), m_dir(direction)
    {
    }

    dvec3 origin() const { return m_orig; }
    dvec3 direction() const { return m_dir; }

    dvec3 at(double t) const
    {
        return m_orig + t * m_dir;
    }


};



