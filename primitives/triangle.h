#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../hittable.h"
#include "../interval.h"
#include "../material.h"
#include "../utils.h"
#include "../vec3.h"
#include <cmath>
#include <iostream>
#include <memory>

class triangle : public hittable {
public:
  triangle(point3 a, point3 b, point3 c, shared_ptr<material> _material)
      : p0(a), p1(b), p2(c), mat(_material) {}

  bool hit(const ray &r, interval ray_dist, hit_record &rec) const override {
    vec3 e1 = p1 - p0;
    vec3 e2 = p2 - p0;
    vec3 N = cross(e1, e2);

    // Skip parallel
    double NdotDir = dot(N, r.direction());
    if (interval(-epsilon, epsilon).contains(NdotDir))
      return false;

    double t = dot(N, p0 - r.origin()) / NdotDir;

    // Skip behind camera
    if (t <= 0.0001)
      return false;

    vec3 p = r.at(t);

    // Skip outside triangle
    if (dot(N, cross(e1, p - p0)) < 0)
      return false;
    if (dot(N, cross(p2 - p1, p - p1)) < 0)
      return false;
    if (dot(N, cross(p0 - p2, p - p2)) < 0)
      return false;

    rec.t = t;
    rec.p = p;
    rec.set_face_normal(r, unit_vector(N));
    rec.mat = mat;

    return true;
  }

private:
  point3 p0, p1, p2;
  shared_ptr<material> mat;
};

#endif // !TRIANGLE_H
