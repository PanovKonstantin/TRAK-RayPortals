//
// Created by mateusz on 19.01.24.
//

#ifndef TRAK_RAYPORTALS_PORTAL_H
#define TRAK_RAYPORTALS_PORTAL_H

#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "utils.h"
#include "vec3.h"
#include "primitives/plane.h"
#include "primitives/triangle.h"
#include <cmath>
#include <memory>

class portal_in : public material {
public:
  portal_in(const std::shared_ptr<triangle> &t) {
    auto points = t->get_points();
    out_origin = points[0];
    out_u = points[1] - points[0];
    out_v = points[2] - points[1];
    out_normal = t->get_normal();
  }

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    scattered = ray(rec.p, r_in.direction());
    attenuation = color(1, 1, 1);
    return true;
  }

  bool teleport(const ray &r_in, const hit_record &rec, ray &scattered,
                int depth, int max_depth) const override {

    // Skip rays that come directly from the camera
    if (max_depth == depth)
      return false;

    scattered = ray(out_origin + rec.u * out_u + rec.v * out_v,
                    out_normal + (rec.normal - r_in.direction()));
    return true;
  }

private:
  vec3 out_normal, out_v, out_u;
  point3 out_origin;
};

class portal_out : public material {
  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    scattered = ray(rec.p, r_in.direction());
    attenuation = color(1, 1, 1);
    return true;
  }
};

#endif // TRAK_RAYPORTALS_PORTAL_H
