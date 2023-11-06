#include "util.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

Image::Image(int width, int height)
{
    this->m_width = width;
    this->m_height = height;
    m_data = new Color[width * height];
}
Image::~Image()
{
    delete[] m_data;
}
void Image::setPixel(int x, int y, Color c)
{
    m_data[y * m_width + x] = c;
}
void Image::write(const char *filename, int spp)
{
    unsigned char* output_data = new unsigned char [m_width * m_height * 3];
    double scale = 1. / spp;
    for (int i = 0; i < m_width * m_height; ++i)
    {
        Color c = m_data[i];
        auto r = c.r;
        auto g = c.g;
        auto b = c.b;

        r = sqrt(r * scale);
        g = sqrt(g * scale);
        b = sqrt(b * scale);

        
        unsigned char ir = static_cast<unsigned char>(256 * clamp(r, 0.0, 0.999));
        unsigned char ig = static_cast<unsigned char>(256 * clamp(g, 0.0, 0.999));
        unsigned char ib = static_cast<unsigned char>(256 * clamp(b, 0.0, 0.999));
        
        output_data[i * 3 + 0] = ir;
        output_data[i * 3 + 1] = ig;
        output_data[i * 3 + 2] = ib;
    }
    std::cout << "Writing to " << filename << std::endl;
    stbi_write_png(filename, m_width, m_height, 3, output_data, m_width * 3);
}

unsigned char* load(const char *filename, int bpp, int& width, int& height)
{
    auto components_per_pixel = bpp;
    return stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
}


std::ostream &operator<<(std::ostream &os, dvec3 const &a) {
    os << "{";
    for (size_t i = 0; i < a.length(); i++) {
        os << a[i];
        if (i != a.length() - 1)
            os << ", ";
    }
    os << "}";
    return os;
}

dvec3 uniform_sample_hemisphere(double& pdf)
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);

    auto phi = 2*PI*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    pdf = invPI;

    return dvec3(x, y, z);
}

dvec2 concentric_sample_disk()
{
    auto u1 = random_double();
    auto u2 = random_double();

    u1 = u1*2.0 - 1.0;
    u2 = u2*2.0 - 1.0;

    if (u1 == 0 && u2 == 0) return dvec2(0, 0);

    double theta, r;
    if (std::abs(u1) > std::abs(u2))
    {
        r = u1;
        theta = (PI / 4) * (u2 / u1);
    }
    else
    {
        r = u2;
        theta = (PI / 2) - (PI / 4) * (u1 / u2);
    }
    return dvec2(r*cos(theta), r*sin(theta));
}

dvec3 cosine_sample_hemisphere(double& pdf)
{
    dvec2 d = concentric_sample_disk();
    double z = sqrt(std::max(0.0, 1-d.x*d.x-d.y*d.y));
    pdf = z * invPI;
    return z < 0.001 ? dvec3(0., 0., 0.) : dvec3(d.x, d.y, z);
}

dvec3 random_cosine_direction(double& pdf) 
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);

    auto phi = 2*PI*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    pdf =  z * invPI;

    return dvec3(x, y, z);
}