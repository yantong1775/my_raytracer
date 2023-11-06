#include <iostream>

#include "core/ray.h"
#include "shape/sphere.h"
#include "util.h"
#include "core/camera.h"
#include "core/hittable_list.h"
#include "acceleration/bvh.h"
#include "shape/transformation.h"

#include "scene/scene.h"
#include "emit/arealight.h"

using std::cout;
using std::endl;

using std::make_shared;


Color ray_color(const ray& r, const Color background_color, const hittable_list& root, const shared_ptr<AreaLight> light, int depth) 
{
    hit_record rec;
    if (depth <= 0) return Color(0, 0, 0);

    if (!root.hit(r, 0.001, INF, rec)) return background_color;

    // Define a coordinate space for the hit point.
    dmat3 o2w;
    make_coord_space(o2w, rec.m_normal);
    dmat3 w2o = transpose(o2w);

    // w_in and w_out are in the object space.
    // w_in is defined as the direction from the hit point to the origin.
    dvec3 w_in = w2o * (-r.m_dir);
    dvec3 w_out;

    dvec3 f_r;
    double pdf;

    Color emit = rec.m_mat_ptr->emitted(rec.m_u, rec.m_v, rec.m_p);

    if (!rec.m_mat_ptr->scatter(w_in, rec, f_r, w_out, pdf)) 
        return emit;

    light->sample_L(rec.m_p, w_out, rec.m_t, pdf);
    
    ray scatter_ray = ray(rec.m_p, o2w * w_out);
    
    return emit + f_r * w_out.z * ray_color(scatter_ray, background_color, root, light, depth-1) / pdf;

}

int main()
{
    
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 500;
    int samples_per_pixel = 100;
    const int max_depth = 50;
    Color background(0,0,0);
    hittable_list objects;

    dvec3 lookfrom;
    dvec3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    // object list
    switch(0)
    {
        case 1:
            objects = random_scene();
            background = Color(0.70, 0.80, 1.00);
            lookfrom = dvec3(13,2,3);
            lookat = dvec3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2:
            objects = two_spheres();
            background = Color(0.70, 0.80, 1.00);
            lookfrom = dvec3(13,2,3);
            lookat = dvec3(0,0,0);
            vfov = 20.0;
            break;

        case 3:
            objects = two_perlin_spheres();
            background = Color(0.70, 0.80, 1.00);
            lookfrom = dvec3(13,2,3);
            lookat = dvec3(0,0,0);
            vfov = 20.0;
            break;
        case 4:
            objects = earth();
            background = Color(0.70, 0.80, 1.00);
            lookfrom = dvec3(13,2,3);
            lookat = dvec3(0,0,0);
            vfov = 20.0;
            break;
        // default:
        case 5:
            objects = simple_light();
            samples_per_pixel = 400;
            background = Color(0,0,0);
            lookfrom = dvec3(26,3,6);
            lookat = dvec3(0,2,0);
            vfov = 20.0;
            break;
        default:
        case 6:
            objects = cornell_box();
            aspect_ratio = 1.0;
            image_width = 500;
            samples_per_pixel = 500;
            background = Color(0,0,0);
            lookfrom = dvec3(278, 278, -800);
            lookat = dvec3(278, 278, 0);
            vfov = 40.0;
            break;

        // default:
        case 7:
            objects = cornell_smoke();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            lookfrom = dvec3(278, 278, -800);
            lookat = dvec3(278, 278, 0);
            vfov = 40.0;
            break;
    }

    hittable_list world;
    world.add(make_shared<bvh_node>(objects, 0.0, 1.0));

    shared_ptr<AreaLight> light = make_shared<AreaLight>(dvec3(7, 7, 7), dvec3(278, 279.5, 554), dvec3(0, -1, 0), dvec3(330, 0, 0), dvec3(0, 0, 305));

    // Camera
    

    dvec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);


    Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

    Image image(image_width, image_height);
    std::cout << "start rendering" << std::endl;
    for (int j = image_height-1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            Color c{0, 0, 0};
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = double(i + random_double()) / (image_width-1);
                auto v = double(j + random_double()) / (image_height-1);
                ray r = cam.generate_ray(u, v);
                c += ray_color(r, background, world, light, max_depth);                
            }
            image.setPixel(i, image_height - 1 - j, c);
        }
    }
    image.write("../src/result/cornell_box_500_500_cosine_hemi.png", samples_per_pixel);
}