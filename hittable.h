#ifndef HITTABLE_H
#define HITTABLE_H

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "utils.h"

class hit_record;

class material {
public:
  virtual ~material() = default;
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scattered) const = 0;
};

class hit_record {
public:
  point3 p;
  vec3 normal;
  shared_ptr<material> mat;
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
