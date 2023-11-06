#include "transformation.h"

bool translate::hit(const ray& r, 
    double t_min, double t_max, hit_record& rec) const
{
    ray moved_r(r.m_orig - m_offset, r.m_dir);

    if (!m_ptr->hit(moved_r, t_min, t_max, rec)) return false;

    rec.m_p = rec.m_p + m_offset;
    rec.set_face_normal(moved_r, rec.m_normal);

    return true;
}

bool translate::bounding_box(double time0, double time1,
        aabb& output_box) const
{
    if (!m_ptr->bounding_box(time0, time1, output_box))
    return false;

    output_box = aabb(
        output_box.min() + m_offset,
        output_box.max() + m_offset);

    return true;
}



rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : ptr(p) 
{
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    dvec3 min( INF,  INF,  INF);
    dvec3 max(-INF, -INF, -INF);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.m_max.x + (1-i)*bbox.m_min.x;
                auto y = j*bbox.m_max.y + (1-j)*bbox.m_min.y;
                auto z = k*bbox.m_max.z + (1-k)*bbox.m_min.z;

                auto newx =  cos_theta*x + sin_theta*z;
                auto newz = -sin_theta*x + cos_theta*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(
    const ray& r, double t_min, double t_max, hit_record& rec) const
{
    auto origin = r.m_orig;
    auto direction = r.m_dir;

    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction);

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.m_p;
    auto normal = rec.m_normal;

    p[0] =  cos_theta*rec.m_p[0] + sin_theta*rec.m_p[2];
    p[2] = -sin_theta*rec.m_p[0] + cos_theta*rec.m_p[2];

    normal[0] =  cos_theta*rec.m_normal[0] + sin_theta*rec.m_normal[2];
    normal[2] = -sin_theta*rec.m_normal[0] + cos_theta*rec.m_normal[2];

    rec.m_p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

bool rotate_y::bounding_box(double time0, double time1, aabb& output_box) const 
{
    output_box = bbox;
    return hasbox;
}


void make_coord_space(dmat3 &o2w, const dvec3 n) 
{

  dvec3 z = dvec3(n.x, n.y, n.z);
  dvec3 h = z;
  if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z))
    h.x = 1.0;
  else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z))
    h.y = 1.0;
  else
    h.z = 1.0;

  z = normalize(z);
  dvec3 y = cross(h, z);
  y = normalize(y);
  dvec3 x = cross(z, y);
  x = normalize(x);

  o2w[0] = x;
  o2w[1] = y;
  o2w[2] = z;
}