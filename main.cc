#include <memory>
#include <windows.h>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
#include "sphere.h"
#include "utils.h"

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

  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dielectric>(1.5);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

  world.add(
      make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  camera cam;
  cam.aspect_ratio = 16. / 9.;

  // quick

  // cam.image_width = 300;
  // cam.max_depth = 5;
  // cam.sample_per_pixel = 5;

  // better

  cam.image_width = 400;
  cam.max_depth = 50;
  //
  // best

  // cam.image_width = 1920;
  // cam.max_depth = 50;

  cam.render(world);

  return 0;
}
