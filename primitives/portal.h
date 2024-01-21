//
// Created by mateusz on 19.01.24.
//

#ifndef TRAK_RAYPORTALS_PORTAL_H
#define TRAK_RAYPORTALS_PORTAL_H

#include "../hittable.h"
#include "../interval.h"
#include "../material.h"
#include "../utils.h"
#include "../vec3.h"
#include "triangle.h"
#include "plane.h"
#include <cmath>
#include <memory>

class portal : public hittable {
public:
    portal(triangle &entry_plane, triangle &exit_plane)
            : entry(entry_plane), exit(exit_plane) {}

    bool hit(const ray &r, interval ray_dist, hit_record &rec) const override {
        bool hit = exit.hit(r, ray_dist, rec);

        if (hit) {
            set_entry_hit_record(r, rec);
        }
        return hit;
    }

private:
    triangle entry, exit;

    point3 get_point_from_exit(point3 hit_point) const {
        auto entry_points = entry.get_points();
        auto exit_points = exit.get_points();

        vec3 bc = barycentric_coordinates(exit_points[0], exit_points[1], exit_points[2], hit_point);

        double a = bc[0] * entry_points[0].x() + bc[1] * entry_points[1].x() + bc[2] * entry_points[2].x();
        double b = bc[0] * entry_points[0].y() + bc[1] * entry_points[1].y() + bc[2] * entry_points[2].y();
        double c = bc[0] * entry_points[0].z() + bc[1] * entry_points[1].z() + bc[2] * entry_points[2].z();

        return point3(a, b, c);
    }

    void set_entry_hit_record(const ray &r, hit_record &rec) const {
        auto entry_points = entry.get_points();

        vec3 e1 = entry_points[1] - entry_points[0];
        vec3 e2 = entry_points[2] - entry_points[0];
        vec3 s = r.origin() - entry_points[0];

        double a = dot(e1, -cross(s, e2));
        double b = dot(e1, -cross(-r.direction(), e2));
        double t = a / b;

        rec.p = get_point_from_exit(rec.p);
        rec.t = t;
        rec.set_face_normal(r, unit_vector(entry.get_normal()));
        rec.mat = entry.get_material();
    }
};

#endif //TRAK_RAYPORTALS_PORTAL_H
