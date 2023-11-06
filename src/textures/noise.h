#pragma once

#include "../util.h"
#include "texture.h"
#include "perlin_noise.h"

class noise_texture : public texture
{
public:
    perlin m_noise;
    double m_scale;

public:
    noise_texture() {}
    noise_texture(double sc) : m_scale(sc) {}

    virtual Color value(double u, double v, const dvec3& p) const override
    {
        return Color(1.,1.,1.) * 0.5 * 
        (1.0 + sin(m_scale*p.z + 10*m_noise.turbulence(p)));
    }
};