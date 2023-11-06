#pragma once
#include "texture.h"

class solid_color : public texture
{
    
public: 
    Color m_color;

public:
    solid_color() {}
    solid_color(Color c) : m_color(c) {}

    solid_color(double r, double g, double b) : solid_color(Color(r, g, b)) {}

    virtual Color value(double u, double v, const dvec3 &p) const override
    {
        return m_color;
    }
};