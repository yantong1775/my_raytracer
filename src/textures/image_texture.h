#pragma once
#include "../util.h"
#include "texture.h"

class image_texture : public texture
{
private:
    shared_ptr<unsigned char> m_data;
    int m_width, m_height;
    int m_bytes_per_scanline;

public:
    const static int byte_per_pixel = 3;
    
    image_texture()
        : m_data(nullptr), m_width(0), m_height(0), m_bytes_per_scanline(0) {}

    image_texture(const char* filename);

    virtual Color value(double u, double v, const dvec3& p) const override;
};