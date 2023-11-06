#pragma once

#include "../core/hittable.h"

class xy_rect : public hittable
{
public:
    double m_x0, m_x1, m_y0, m_y1, m_z;
    shared_ptr<Material> m_mat_ptr;

public:
    xy_rect() {}
    xy_rect(double x0, double x1, double y0, double y1, double z, shared_ptr<Material> mat_ptr)
        : m_x0(x0), m_x1(x1), m_y0(y0), m_y1(y1), m_z(z), m_mat_ptr(mat_ptr) {}

    virtual bool hit(const ray& r, double t_min, double t_max, 
                hit_record& rec) const override
    {
        auto t = (m_z - r.m_orig.z) / r.direction().z;

        if (t < t_min || t > t_max) return false;

        auto x = r.m_orig.x + t*r.direction().x;
        auto y = r.m_orig.y + t*r.direction().y;

        if (x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1)
            return false;
        
        rec.m_u = (x - m_x0) / (m_x1 - m_x0);
        rec.m_v = (y - m_y0) / (m_y1 - m_y0);

        rec.m_t = t;
        auto normal = dvec3(0, 0, 1);
        rec.set_face_normal(r, normal);
        rec.m_mat_ptr = m_mat_ptr;
        rec.m_p = r.at(t);
        return true;
    }


    virtual bool bounding_box(double time0, double time1, 
                aabb& output_box) const override
    {
        output_box = aabb(dvec3(m_x0,m_y0, m_z-0.0001), dvec3(m_x1,m_y1, m_z+0.0001));
        return true;
    }

};

class yz_rect : public hittable
{
public:
    double m_y0, m_y1, m_z0, m_z1, m_x;
    shared_ptr<Material> m_mat_ptr;

public:
    
    yz_rect() {}
    yz_rect(double y0, double y1, double z0, double z1, double x, shared_ptr<Material> mat_ptr)
        : m_y0(y0), m_y1(y1), m_z0(z0), m_z1(z1), m_x(x), m_mat_ptr(mat_ptr) {}

    virtual bool hit(const ray& r, double t_min, double t_max, 
                hit_record& rec) const override
    {
        auto t = (m_x - r.m_orig.x) / r.direction().x;

        if (t < t_min || t > t_max) return false;

        auto y = r.m_orig.y + t*r.direction().y;
        auto z = r.m_orig.z + t*r.direction().z;

        if (y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1)
            return false;

        rec.m_u = (y - m_y0) / (m_y1 - m_y0);
        rec.m_v = (z - m_z0) / (m_z1 - m_z0);

        rec.m_t = t;
        auto normal = dvec3(1, 0, 0);
        rec.set_face_normal(r, normal);
        rec.m_mat_ptr = m_mat_ptr;
        rec.m_p = r.at(t);
        return true;
    }

    
    virtual bool bounding_box(double time0, double time1, 
                aabb& output_box) const override
    {
        output_box = aabb(dvec3(m_x - 0.0001, m_y0, m_z0), dvec3(m_x + 0.0001, m_y1, m_z1));
        return true;
    }

};

class xz_rect : public hittable
{
public:
    double m_x0, m_x1, m_z0, m_z1, m_y;
    shared_ptr<Material> m_mat_ptr;
public:
    xz_rect() {}
    xz_rect(double x0, double x1, double z0, double z1, double y, shared_ptr<Material> mat_ptr)
        : m_x0(x0), m_x1(x1), m_z0(z0), m_z1(z1), m_y(y), m_mat_ptr(mat_ptr) {}

    virtual bool hit(const ray& r, double t_min, double t_max, 
                hit_record& rec) const override
    {
        auto t = (m_y - r.m_orig.y) / r.direction().y;
        
        if (t < t_min || t > t_max) return false;

        auto x = r.m_orig.x + t*r.direction().x;
        auto z = r.m_orig.z + t*r.direction().z;

        if (x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1)
            return false;

        rec.m_u = (x - m_x0) / (m_x1 - m_x0);
        rec.m_v = (z - m_z0) / (m_z1 - m_z0);

        rec.m_t = t;
        auto normal = dvec3(0, 1, 0);
        rec.set_face_normal(r, normal);
        rec.m_mat_ptr = m_mat_ptr;
        rec.m_p = r.at(t);
        return true;
    }

    virtual bool bounding_box(double time0, double time1, 
                aabb& output_box) const override
    {
        output_box = aabb(dvec3(m_x0, m_y - 0.0001, m_z0), dvec3(m_x1, m_y + 0.0001, m_z1));
        return true;
    }

};

