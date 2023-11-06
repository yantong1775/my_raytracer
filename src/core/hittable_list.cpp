#include "hittable_list.h"


bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : m_objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.m_t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const
{
    if (m_objects.empty()) return false;

    for (auto object : m_objects)
    {
        aabb temp_bbox;
        if (!object->bounding_box(time0, time1, temp_bbox)) return false;

        output_box.expand(temp_bbox);
    }
    return true;
}