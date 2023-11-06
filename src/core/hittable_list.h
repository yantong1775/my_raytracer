#pragma once

#include "hittable.h"
#include <memory>
#include <vector>

class hittable_list : public hittable
{

public: 
    std::vector<std::shared_ptr<hittable>> m_objects;

public:
    hittable_list() {};
    hittable_list(std::shared_ptr<hittable> object) {add(object);}

    void clear() {m_objects.clear();}
    void add(std::shared_ptr<hittable> object) {m_objects.push_back(object);}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

};

