#pragma once

#include "texture.h"
#include "solid_color.h"

class checker_texture : public texture
{
public:
    shared_ptr<texture> m_odd;
    shared_ptr<texture> m_even;
public:
    checker_texture() {};
    checker_texture(shared_ptr<texture> odd, shared_ptr<texture> even)
        : m_odd(odd), m_even(even) {}
    checker_texture(Color c1, Color c2)
        : m_odd(make_shared<solid_color>(c1)), m_even(make_shared<solid_color>(c2)) {}
    virtual Color value(double u, double v, const dvec3& p) const override
    {
        auto sines = sin(10*p.x) * sin(10*p.y) * sin(10*p.z);
        // std::cout << "checker texture value" << std::endl;
        if (sines < 0)
            return m_odd->value(u, v, p);
        else
            return m_even->value(u, v, p);
    }
};
