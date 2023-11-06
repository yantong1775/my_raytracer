#pragma once

#include "../util.h"

#include "../shape/sphere.h"
#include "../shape/aarect.h"
#include "../shape/box.h"
#include "../shape/transformation.h"
#include "../shape/constant_medium.h"

#include "../core/hittable_list.h"


#include "../material/lambertian.h"
#include "../material/mirror.h"
#include "../material/fuzzy.h"
#include "../material/dielectric.h"
#include "../material/light.h"


#include "../textures/solid_color.h"
#include "../textures/checker.h"
#include "../textures/noise.h"
#include "../textures/image_texture.h"

hittable_list random_scene() {
    hittable_list world;
    
    auto checker = make_shared<checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(dvec3(0,-1000,0), 1000, make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            dvec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - dvec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = dvec3(random_double(), random_double(), random_double()) * dvec3(random_double(), random_double(), random_double());
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = dvec3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Fuzzy>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<sphere>(dvec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(dvec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Fuzzy>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(dvec3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list two_spheres()
{
    hittable_list objects;

    auto checker = make_shared<checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(dvec3(0,-10, 0), 10, make_shared<Lambertian>(checker)));
    objects.add(make_shared<sphere>(dvec3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres() {
    
    hittable_list objects;
    auto pertext = make_shared<noise_texture>(4.);
    auto checker = make_shared<checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(dvec3(0,-1000,0), 1000, make_shared<Lambertian>(checker)));
    objects.add(make_shared<sphere>(dvec3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    return objects;
}

hittable_list earth() {
    
    auto earth_texture = make_shared<image_texture>("../resources/texture/earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<sphere>(dvec3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(dvec3(0,-1000,0), 1000, make_shared<Lambertian>(pertext)));
    objects.add(make_shared<sphere>(dvec3(0,2,0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(Color(4,4,4));
    objects.add(make_shared<xy_rect>(3., 5., 1., 3., 2., difflight));
    objects.add(make_shared<sphere>(dvec3(0, 8, 0), 2, difflight));
    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;
    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(Color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    // objects.add(make_shared<sphere>(dvec3(278, 278, 200), 40, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(dvec3(0, 0, 0), dvec3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(dvec3(0,0,0), dvec3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    objects.add(box2);

    return objects;
}

hittable_list cornell_smoke() {
    hittable_list objects;

    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(Color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(dvec3(0,0,0), dvec3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = make_shared<box>(dvec3(0,0,0), dvec3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    objects.add(make_shared<constant_medium>(box1, 0.01, Color(0,0,0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, Color(1,1,1)));

    return objects;
}