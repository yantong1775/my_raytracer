#include "image_texture.h"


image_texture::image_texture(const char* filename)
{
    auto components_per_pixel = byte_per_pixel;
    m_data = shared_ptr<unsigned char>(load(filename, byte_per_pixel, m_width, m_height));

    if (!m_data)
    {
        std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
        m_width = m_height = 0;
    }

    m_bytes_per_scanline = byte_per_pixel * m_width;

}   

Color image_texture::value(double u, double v, const dvec3& p) const
{
    
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (m_data == nullptr)
        return Color(0, 1, 1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

    auto i = static_cast<int>(u * m_width);
    auto j = static_cast<int>(v * m_height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= m_width)  i = m_width - 1;
    if (j >= m_height) j = m_height - 1;

    const auto color_scale = 1.0 / 255.0;
    auto pixel = m_data.get() + j * m_bytes_per_scanline + i * byte_per_pixel;

    return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);

}