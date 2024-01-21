#include <memory>
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "primitives/plane.h"
#include "primitives/sphere.h"
#include "vec3.h"
#include "primitives/portal.h"


void simple_light() {
    hittable_list world;

    auto ground = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto red = make_shared<lambertian>(color(1, 0, 0));
    auto pink = make_shared<lambertian>(color(0.8, 0.4, 0.4));
    auto mirror = make_shared<metal>(color(.9, 1, .9), 0);
    auto light = make_shared<diffuse_light>(color(4, 4, 4));
    auto portal_mat = make_shared<portal_material>();

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, ground));

    auto entry = triangle(point3(3, 1, 2), point3(5, 1, 2), point3(3, 3, 2), portal_mat);

    auto exit = triangle(point3(3, 1, 4), point3(5, 1, 4), point3(3, 3, 4), portal_mat);

    world.add(std::make_shared<portal>(entry, exit));

    auto difflight = make_shared<diffuse_light>(color(8, 8, 8));
    world.add(
            make_shared<plane>(new point3[4]{point3(3, 1, -2), point3(5, 1, -2),
                                             point3(3, 3, -2), point3(5, 3, -2)},
                               difflight));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 20;
    cam.lookfrom = point3(26, 3, 6);
    cam.lookat = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_box() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0),
                                vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555),
                                red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                                vec3(0, 0, -105), light));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555),
                                white));
    world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0),
                                vec3(0, 0, -555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0),
                                vec3(0, 555, 0), white));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 300;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    switch (1) {
        case 1:
            simple_light();
            break;
        case 2:
            cornell_box();
            break;
    }
}
