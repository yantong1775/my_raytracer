#include "bvh.h"

#define BINS 8

inline bool box_pos_compare(const shared_ptr<hittable> a, const double pos, int axis)
{
    aabb box;
    if (!a->bounding_box(0,0, box))
        std::cerr << "No bounding box in bvh_node constructor.\n";
    return box.centroid()[axis] < pos;
}

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0,0, box_a) || !b->bounding_box(0,0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.centroid()[axis] < box_b.centroid()[axis];
}


bool box_pos_x_compare (const shared_ptr<hittable> a, const double pos) {
    return box_pos_compare(a, pos, 0);
}

bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_pos_y_compare (const shared_ptr<hittable> a, const double pos) {
    return box_pos_compare(a, pos, 1);
}

bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_pos_z_compare (const shared_ptr<hittable> a, const double pos) {
    return box_pos_compare(a, pos, 2);
}

bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const
{
    output_box = m_box;
    return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    bool hit = false;

    if (!m_box.hit(r, t_min, t_max)) return hit;
    

    bool hit_left = m_left->hit(r, t_min, t_max, rec);
    t_max = hit_left ? rec.m_t : t_max;
    bool hit_right = m_right->hit(r, t_min, t_max, rec);

    return hit_left || hit_right;    
}


struct Bin
{
    aabb m_box;
    int count;
};

double bvh_node::find_best_split(
    std::vector<shared_ptr<hittable>>& src_objects, 
    int& axis, double& splitPos, size_t start, size_t end,
    double time0, double time1)
{
   double bestCost = INF;   
   /*int BINS = 8;*/
   for (int a = 0; a < 3; a++)
   {
        double boundsMin = INF, boundsMax = -INF;
        for (size_t idx = start; idx < end; idx++)
        {
            auto obj = src_objects[idx];
            aabb temp_box;
            obj->bounding_box(time0, time1, temp_box);
            boundsMin = std::min(boundsMin, temp_box.centroid()[a]);
            boundsMax = std::max(boundsMax, temp_box.centroid()[a]);
        }

        // std::cout << "bound min " << boundsMin << " " << "bounds max " << boundsMax << std::endl;

        if (boundsMin == boundsMax) continue;
        // populate the bins
        Bin bin[BINS];
        double scale = BINS / (boundsMax - boundsMin);
        
        for (size_t idx = start; idx < end; idx++)
        {
            auto obj = src_objects[idx];
            aabb temp_box;
            obj->bounding_box(time0, time1, temp_box);
            int bin_idx = std::min( BINS - 1, 
                (int)((temp_box.centroid()[a] - boundsMin) * scale));
            bin[bin_idx].count++;
            bin[bin_idx].m_box.expand(temp_box);
        }

        // gather data for the 7 planes between the 8 bins
        double leftArea[BINS - 1], rightArea[BINS - 1];
        int leftCount[BINS - 1], rightCount[BINS - 1];
        aabb leftBox, rightBox;
        int leftSum = 0, rightSum = 0;
        for (int i = 0; i < BINS - 1; i++)
        {
            leftSum += bin[i].count;
            leftCount[i] = leftSum;
            leftBox.expand( bin[i].m_box );
            leftArea[i] = leftBox.surface_area();
            rightSum += bin[BINS - 1 - i].count;
            rightCount[BINS - 2 - i] = rightSum;
            rightBox.expand( bin[BINS - 1 - i].m_box );
            rightArea[BINS - 2 - i] = rightBox.surface_area();
        }
        // calculate SAH cost for the 7 planes
        scale = (boundsMax - boundsMin) / BINS;
        for (int i = 0; i < BINS - 1; i++)
        {
            double planeCost = 
            leftCount[i] * leftArea[i] + rightCount[i] * rightArea[i];
            if (planeCost < bestCost)
                axis = a, splitPos = boundsMin + scale * (i + 1), 
            bestCost = planeCost;
        }
   }
   return bestCost;
}

bvh_node::bvh_node(
    const std::vector<shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, double time0, double time1)
{

    int best_axis = 0;
    double best_split_pos = 0;
    auto objects = src_objects;

    double best_cost = find_best_split(
        objects, best_axis, best_split_pos,
        start, end, time0, time1
    );

    auto comparator = (best_axis == 0) ? box_x_compare
                    : (best_axis == 1) ? box_y_compare
                                       : box_z_compare;

    auto pos_comparator = (best_axis == 0) ? box_pos_x_compare
                        : (best_axis == 1) ? box_pos_y_compare
                                           : box_pos_z_compare;
    
    size_t obj_span = end - start;

    if (obj_span == 1) m_left = m_right = objects[start];
    else if (obj_span == 2) 
    {
        if (comparator(objects[start], objects[start+1]))
        {
            m_left = objects[start];
            m_right = objects[start+1];
        }
        else
        {
            m_left = objects[start+1];
            m_right = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        
        auto iter = std::lower_bound(
            objects.begin()+start, objects.begin()+end, 
            best_split_pos, pos_comparator);

        int idx = iter - objects.begin();

        m_left = std::make_shared<bvh_node>(objects, start, idx, time0, time1);
        m_right = std::make_shared<bvh_node>(objects, idx, end, time0, time1);
    }

    aabb box_left, box_right;

    if (  !m_left->bounding_box (time0, time1, box_left)
       || !m_right->bounding_box(time0, time1, box_right)
    ) 
        std::cerr << "No bounding box in bvh_node constructor.\n";
    
    m_box.expand(box_left);
    m_box.expand(box_right);

}
