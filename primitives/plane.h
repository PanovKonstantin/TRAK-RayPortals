#ifndef PLANE_H
#define PLANE_H

#include "../hittable.h"
#include "../interval.h"
#include "../material.h"
#include "../utils.h"
#include "../vec3.h"
#include "triangle.h"
#include <cmath>
#include <memory>

class plane : public hittable {
public:
  plane(point3 p[4], shared_ptr<material> _material)
      : t1(p[0], p[2], p[1], _material), t2(p[1], p[2], p[3], _material) {}

  plane(point3 o, vec3 u, vec3 v, shared_ptr<material> _material)
      : t1(o, o + u, o + v, _material), t2(o + v + u, o + v, o + u, _material) {
  }

  bool hit(const ray &r, interval ray_dist, hit_record &rec) const override {
    if (t1.hit(r, ray_dist, rec))
      return true;
    if (t2.hit(r, ray_dist, rec))
      return true;
    return false;
  }

private:
  triangle t1, t2;
};

using quad = plane;

#endif // !PLANE_H
