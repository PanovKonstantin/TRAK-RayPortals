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
#include <array>

class triangle : public hittable {
public:
    triangle(point3 a, point3 b, point3 c, shared_ptr<material> _material)
            : p0(a), p1(b), p2(c), mat(_material) {
        vec3 e1 = p1 - p0;
        vec3 e2 = p2 - p0;
        normal_vector = unit_vector(cross(e1, e2));
    }

    bool hit(const ray &r, interval ray_dist, hit_record &rec) const override {
        vec3 e1 = p1 - p0;
        vec3 e2 = p2 - p0;
        vec3 s = r.origin() - p0;

        // Skip parallel
        double NdotDir = dot(normal_vector, r.direction());
        if (interval(-epsilon, epsilon).contains(NdotDir))
            return false;

        double a = dot(e1, -cross(s, e2));
        double b = dot(e1, -cross(-r.direction(), e2));
        double t = a / b;

        // Skip behind camera
        if (!ray_dist.contains(t))
            return false;

        vec3 p = r.at(t);

        // Skip outside triangle
        if (dot(normal_vector, cross(e1, p - p0)) < 0)
            return false;
        if (dot(normal_vector, cross(p2 - p1, p - p1)) < 0)
            return false;
        if (dot(normal_vector, cross(p0 - p2, p - p2)) < 0)
            return false;

        rec.t = t;
        rec.p = p;
        rec.set_face_normal(r, unit_vector(normal_vector));
        rec.mat = mat;

        return true;
    }

    vec3 get_normal() const {
        return normal_vector;
    }

    std::array<point3, 3> get_points() const {
        std::array<point3, 3> points;

        points[0] = p0;
        points[1] = p1;
        points[2] = p2;

        return points;
    }

    shared_ptr<material> get_material() const {
        return mat;
    }

private:
    point3 p0, p1, p2;
    vec3 normal_vector;
    shared_ptr<material> mat;
};

#endif // !TRIANGLE_H
