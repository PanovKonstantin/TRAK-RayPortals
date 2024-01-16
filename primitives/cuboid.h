#ifndef CUBOID_H
#define CUBOID_H

#include "../hittable.h"
#include "../interval.h"
#include "../material.h"
#include "../utils.h"
#include "../vec3.h"
#include "triangle.h"
#include <cmath>
#include <memory>

class cuboid : public hittable {
public:
    cuboid(point3 p[8], shared_ptr<material> _material)
        : t1(p[0], p[1], p[2], _material), t2(p[1], p[2], p[3], _material), t3(p[2], p[3], p[4], _material), t4(p[3], p[4], p[5], _material), t5(p[4], p[5], p[6], _material), t6(p[5], p[6], p[7], _material), t7(p[6], p[7], p[0], _material), t8(p[7], p[0], p[1], _material) {}

    cuboid(point3 o, vec3 u, vec3 v, vec3 w, shared_ptr<material> _material)
        : t1(o, o + v, o + u, _material), t2(o + v + u, o + v, o + u, _material), t3(o + v + u, o + v + w, o + u + w, _material), t4(o + v + u + w, o + v + w, o + u + w, _material), t5(o + v + u + w, o + v + w + u, o + u + w + v, _material), t6(o + v + u + w + v + u, o + v + w + u, o + u + w + v, _material), t7(o + v + u + w + v + u, o + v + w + u + v + w, o + u + w + v + u + w, _material), t8(o + v + u + w + v + u + w, o + v + w + u + v + w, o + u + w + v + u + w + v, _material) {
    }

    bool hit(const ray &r, interval ray_dist, hit_record &rec) const override {
        if (t1.hit(r, ray_dist, rec))
            return true;
        if (t2.hit(r, ray_dist, rec))
            return true;
        if (t3.hit(r, ray_dist, rec))
            return true;
        if (t4.hit(r, ray_dist, rec))
            return true;
        if (t5.hit(r, ray_dist, rec))
            return true;
        if (t6.hit(r, ray_dist, rec))
            return true;
        if (t7.hit(r, ray_dist, rec))
            return true;
        if (t8.hit(r, ray_dist, rec))
            return true;
        return false;
        }

private:
  triangle t1, t2, t3, t4, t5, t6, t7, t8;
};

using cube = cuboid;

#endif // !CUBOID_H