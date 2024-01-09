#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

class hit_record {
public:
  point3 p;
  vec3 normal;
  bool front_face;
  double t;

  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    // Sets the normal vector. outward_normal must be a unit vector

    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  ~hittable() = default;

  virtual bool hit(const ray &r, interval ray_distance,
                   hit_record &rec) const = 0;
};

#endif // !HITTABLE_H
