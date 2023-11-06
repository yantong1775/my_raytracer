#pragma once

#include "../core/hittable.h"

class translate : public hittable
{
public:
    dvec3 m_offset;
    shared_ptr<hittable> m_ptr;
public:
    translate() {}
    translate(shared_ptr<hittable> p, const dvec3& displacement)
        : m_offset(displacement), m_ptr(p) {}

    virtual bool hit(const ray& r, 
        double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1,
         aabb& output_box) const override;

};

class rotate_y : public hittable 
{
    public:
        rotate_y(shared_ptr<hittable> p, double angle);

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<hittable> ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb bbox;
};

void make_coord_space(dmat3 &o2w, const dvec3 n);