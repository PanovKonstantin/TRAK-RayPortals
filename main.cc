#include <cmath>
#include <iostream>
#include <memory>
#include <windows.h>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "sphere.h"
#include "utils.h"

color ray_color(const ray &r, const hittable &world) {
  hit_record rec;
  // sphere s(point3(0, 0, -1), .5);
  // double t = s.hit(r, -100, 100, hr);
  if (world.hit(r, interval(0, infinity), rec)) {
    return .5 * (rec.normal + vec3(1, 1, 1));
  }
  double a = .5 * (unit_vector(r.direction()).y() + 1);
  return color(1, 1, 1) * (1 - a) + a * color(.5, 0.7, 1);
}

int main(int argc, char *argv[]) {
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), .5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

  camera cam;
  cam.aspect_ratio = 16. / 9.;
  cam.image_width = 400;
  cam.render(world);

  return 0;
}
