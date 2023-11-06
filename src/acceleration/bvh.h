#pragma once

#include "../core/hittable_list.h"
#include <iostream>
#include <algorithm>

using std::shared_ptr;

class bvh_node : public hittable
{
public:
    shared_ptr<hittable> m_left;
    shared_ptr<hittable> m_right;
    size_t m_start, m_end;
    aabb m_box;

public:
    bvh_node();

    bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.m_objects, 0, list.m_objects.size(), time0, time1) {}
    
    bvh_node(
       const std::vector<shared_ptr<hittable>>& src_objects, 
       size_t start, size_t end, double time0, double time1);
    
    double find_best_split(
        std::vector<shared_ptr<hittable>>& src_objects, 
        int& axis, double& splitPos, size_t start, size_t end,
        double time0, double time1);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

