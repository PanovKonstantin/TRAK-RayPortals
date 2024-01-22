#ifndef VEC3_H
#define VEC3_H

#include "utils.h"
#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
  double e[3];
  vec3() : e{0, 0, 0} {}
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }
  vec3 &operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }
  vec3 &operator/=(double t) { return *this *= 1 / t; }

  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  bool near_zero() const {
    auto s = 1e-8;
    return fabs(e[0]) < s && fabs(e[1]) < s && fabs(e[2]) < s;
  }

  double length() const { return sqrt(length_squared()); }

  static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
  }

  static vec3 random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max),
                random_double(min, max));
  }
};

using point3 = vec3;

inline bool operator==(const vec3& u, const vec3& v) {
  return u.e[0] == v.e[0] && u.e[1] == v.e[1] && u.e[2] == v.e[2];
}

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const vec3 &u, double t) {
  return vec3(u.e[0] * t, u.e[1] * t, u.e[2] * t);
}

inline vec3 operator*(double t, const vec3 &u) { return u * t; }

inline vec3 operator/(const vec3 &u, double t) { return (1 / t) * u; }

inline double dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

inline vec3 random_in_unit_sphere() {
  auto p = vec3::random(-1, 1);
  while (p.length_squared() >= 1)
    p = vec3::random(-1, 1);
  return p;
}

inline vec3 random_in_unit_disk() {
  vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
  while (p.length_squared() >= 1)
    p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
  return p;
}

inline vec3 random_unit_vector() {
  return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3 &normal) {
  vec3 vec_on_unit_sphere = random_unit_vector();
  double norm_dot = dot(vec_on_unit_sphere, normal);
  int sign = (norm_dot > 0) - (norm_dot < 0);
  return sign * vec_on_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
  return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3 &v, const vec3 &n, double eta_coef) {
  auto cos_theta = fmin(dot(-v, n), 1.);
  vec3 perp = eta_coef * (v + cos_theta * n);
  vec3 para = -sqrt(fabs(1. - perp.length_squared())) * n;
  return perp + para;
}

inline vec3 barycentric_coordinates(const point3& a, const point3& b, const point3& c, const point3& p) {

  vec3 v0 = b - a, v1 = c - a, v2 = p - a;
  float d00 = dot(v0, v0);
  float d01 = dot(v0, v1);
  float d11 = dot(v1, v1);
  float d20 = dot(v2, v0);
  float d21 = dot(v2, v1);

  double denom = d00 * d11 - d01 * d01;
  double v = (d11 * d20 - d01 * d21) / denom;
  double w = (d00 * d21 - d01 * d20) / denom;
  double u = 1.0f - v - w;

  return vec3(u, v, w);
}

#endif // !VEC3_H
