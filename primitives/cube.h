#ifndef CUBE_H
#define CUBE_H

#include "../hittable.h"
#include "../interval.h"
#include "../material.h"
#include "../utils.h"
#include "../vec3.h"
#include "triangle.h"
#include <cmath>
#include <memory>

class cube : public hittable {
public:
    cube(point3 p[8], shared_ptr<material> _material)
        : t1(p[0], p[1], p[2], _material), t2(p[1], p[2], p[3], _material),
        t3(p[4], p[5], p[6], _material), t4(p[5], p[6], p[7], _material),
        t5(p[0], p[1], p[4], _material), t6(p[1], p[4], p[5], _material),
        t7(p[2], p[3], p[6], _material), t8(p[3], p[6], p[7], _material),
        t9(p[0], p[2], p[4], _material), t10(p[2], p[4], p[6], _material),
        t11(p[1], p[3], p[5], _material), t12(p[3], p[5], p[7], _material) {}
    cube(point3 o, vec3 u, vec3 v, vec3 w, shared_ptr<material> _material)
        : t1(o, o + v, o + u, _material), t2(o + v + u, o + v, o + u, _material),
        t3(o, o + w, o + v, _material), t4(o + w + v, o + w, o + v, _material),
        t5(o, o + u, o + w, _material), t6(o + u + w, o + u, o + w, _material),
        t7(o + u, o + u + w, o + v, _material), t8(o + u + w + v, o + u + w, o + v, _material),
        t9(o + v, o + v + w, o + u, _material), t10(o + v + w + u, o + v + w, o + u, _material),
        t11(o + w, o + w + u, o + v, _material), t12(o + w + u + v, o + w + u, o + v, _material) {}
    
    bool hit(const ray& r, interval ray_dist, hit_record& rec) const override {
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
        if (t9.hit(r, ray_dist, rec))
            return true;
        if (t10.hit(r, ray_dist, rec))
            return true;
        if (t11.hit(r, ray_dist, rec))
            return true;
        if (t12.hit(r, ray_dist, rec))
            return true;
        return false;
    }
    private:
        triangle t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;
};


#endif // !CUBE_H