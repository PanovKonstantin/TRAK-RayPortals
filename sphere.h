#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "interval.h"
#include "vec3.h"
#include <cmath>

class sphere : public hittable {
public:
  sphere(point3 _center, double _radius) : center(_center), radius(_radius) {}

  bool hit(const ray &r, interval ray_dist, hit_record &rec) const override {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto d = half_b * half_b - a * c;
    if (d < 0) {
      return false;
    }
    auto sqrtd = sqrt(d);
    auto root = (-half_b - sqrt(d)) / a;
    if (!ray_dist.surrounds(root)) {
      root = (-half_b + sqrt(d)) / a;
      if (!ray_dist.surrounds(root)) {
        return false;
      }
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, (rec.p - center) / radius);
    return true;
  }

private:
  point3 center;
  double radius;
};

#endif // !SPHERE_H
