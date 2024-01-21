#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"
#include <cmath>

class lambertian : public material {
public:
  lambertian(const color &a) : albedo(a) {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    vec3 scatter_direction = rec.normal + random_unit_vector();
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    }
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

private:
  color albedo;
};

class metal : public material {
public:
  metal(const color &a, double f) : albedo(a), fuzz(f) {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal)) > 0;
  }

private:
  color albedo;
  double fuzz;
};

class dielectric : public material {
public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {

    auto rr = rec.front_face ? 1.0 / ir : ir;
    auto unit_dir = unit_vector(r_in.direction());
    auto cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
    auto sin_theta = sqrt(1 - cos_theta * cos_theta);
    auto cannot_refract = rr * sin_theta > 1.0;
    auto can_reflect = reflectance_chance(cos_theta, rr) > random_double();
    auto dir = cannot_refract || can_reflect
                   ? reflect(unit_dir, rec.normal)
                   : refract(unit_dir, rec.normal, rr);
    attenuation = color(1., 1., 1.);
    scattered = ray(rec.p, dir);
    return true;
  }

private:
  double ir; // Index of refraction

  static double reflectance_chance(double cos, double rr) {
    auto r0 = (1 - rr) / (1 + rr);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cos), 5);
  }
};

class diffuse_light : public material {
public:
  diffuse_light(const color &c) : albedo(c) {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    return false;
  }

  color emitted() const override { return albedo; }

private:
  color albedo;
};

class portal_material : public material {
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        return false;
    }
};

#endif // !MATERIAL_H
