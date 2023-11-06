#pragma once

#include "../util.h"
#include "ray.h"

class Camera
{
private:
    dvec3 m_origin;
    dvec3 m_lower_left_corner;
    dvec3 m_horizontal;
    dvec3 m_vertical;
    dvec3 m_u, m_v, m_w;
    double m_lens_radius;
    
public:
    Camera(
        dvec3 lookfrom, // eye position
        dvec3 lookat, // look at position
        dvec3 vup, // up direction
        double vfov, // vertical field of view
        double aspect_ratio, // aspect ratio
        double aperture, // aperture
        double focus_dist // focus distance
        )
    {
        double theta = degrees_to_radians(vfov);
        double h = tan(theta/2.0);
        
        double half_height = h * focus_dist;
        double half_width = aspect_ratio * half_height;

        m_w = glm::normalize(lookfrom - lookat); // z-axis in camera space
        m_u = glm::normalize(glm::cross(vup, m_w)); // x-axis in camera space
        m_v = glm::cross(m_w, m_u); // y-axis in camera space

        m_origin = lookfrom;

        m_horizontal = 2.0 * half_width * m_u;
        m_vertical = 2.0 * half_height * m_v;
        m_lower_left_corner = m_origin - m_horizontal/2.0 - m_vertical/2.0 - focus_dist * m_w;

        m_lens_radius = aperture / 2.0;
    }

    ray generate_ray(double s, double t) const
    {
        dvec3 rd = m_lens_radius * random_in_unit_disk();
        dvec3 offset = m_u * rd.x + m_v * rd.y;
        return ray(
            m_origin + offset, 
            m_lower_left_corner + s*m_horizontal + t*m_vertical - m_origin - offset);
    }
};