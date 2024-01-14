#include <memory>
#include <windows.h>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
#include "primitives/plane.h"
#include "primitives/sphere.h"
#include "primitives/triangle.h"
#include "utils.h"
#include "vec3.h"

color ray_color(const ray &r, const hittable &world) {
  hit_record rec;
  if (world.hit(r, interval(0.001, infinity), rec)) {
    return .5 * (rec.normal + vec3(1, 1, 1));
  }
  double a = .5 * (unit_vector(r.direction()).y() + 1);
  return color(1, 1, 1) * (1 - a) + a * color(.5, 0.7, 1);
}

int main(int argc, char *argv[]) {
  hittable_list world;

  auto ground = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  auto red = make_shared<lambertian>(color(1, 0, 0));
  auto pink = make_shared<lambertian>(color(0.8, 0.4, 0.4));
  auto mirror = make_shared<metal>(color(.9, 1, .9), 0);

  world.add(make_shared<sphere>(point3(0, -101, 0), 101.0, ground));
  world.add(make_shared<sphere>(point3(1, .2, .5), .1, pink));
  world.add(make_shared<sphere>(point3(-1, 0, -.5), .1, red));

  point3 mirror_points[4] = {point3(0, 0, -1), point3(0, 0, 1),
                             point3(0, 1, -1), point3(0, 1, 1)};
  world.add(make_shared<plane>(mirror_points, mirror));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 10;
  cam.max_depth = 10;

  cam.vfov = 20;
  cam.lookfrom = point3(5, 1, -4);
  cam.lookat = point3(0, .5, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.1;
  cam.focus_dist = 10.0;

  cam.render(world);

  return 0;
}
