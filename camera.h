#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "utils.h"
#include "vec3.h"

class material;
class camera {

public:
  double aspect_ratio = 1;
  int image_width = 100;
  int sample_per_pixel = 100;
  int max_depth = 10;

  void render(const hittable &world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; ++j) {
      std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
                << std::flush;
      for (int i = 0; i < image_width; ++i) {
        color pixel_color(0, 0, 0);
        for (int sample = 0; sample < sample_per_pixel; ++sample) {
          auto r = get_ray(i, j);
          pixel_color += ray_color(r, world, max_depth);
        }
        write_color(std::cout, pixel_color, sample_per_pixel);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  int image_height;
  point3 center;
  point3 pixel_0;
  point3 pixel_delta_u;
  point3 pixel_delta_v;

  void initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = image_height >= 1 ? image_height : 1;

    double focal_length = 1.0;
    double viewport_height = 2.0;
    double actual_ratio = static_cast<double>(image_width) / image_height;
    double viewport_width = viewport_height * actual_ratio;

    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    pixel_0 = center - vec3(0, 0, focal_length) +
              (pixel_delta_u + pixel_delta_v - viewport_v - viewport_u) / 2;
  }

  ray get_ray(int i, int j) {
    auto pixel_center = pixel_0 + (i * pixel_delta_u) + (j * pixel_delta_v);
    auto pixel_sample = pixel_center + pixel_sample_square();
    return ray(center, pixel_sample - center);
  };

  vec3 pixel_sample_square() {
    auto px = -.5 + random_double();
    auto py = -.5 + random_double();
    return (px * pixel_delta_u) + (py * pixel_delta_v);
  };

  color ray_color(const ray &r, const hittable &world, int depth) const {
    hit_record rec;

    if (depth <= 0) {
      return color(0, 0, 0);
    }

    if (world.hit(r, interval(0.001, infinity), rec)) {
      ray scattered;
      color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, world, depth - 1);
      }
      return color(0, 0, 0);
    }

    vec3 unit_dir = unit_vector(r.direction());
    double a = .5 * (unit_dir.y() + 1.);
    return color(1, 1, 1) * (1 - a) + a * color(.5, 0.7, 1);
  }
};

#endif // !CAMERA_H
