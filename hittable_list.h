#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "hittable.h"
#include "interval.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
public:
  std::vector<shared_ptr<hittable>> objects;

  hittable_list(){};
  hittable_list(shared_ptr<hittable> object) { add(object); };

  void add(shared_ptr<hittable> object) { objects.push_back(object); }
  void clear() { objects.clear(); }

  bool hit(const ray &r, interval ray_dist, hit_record &rec) const override {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest = ray_dist.max_lim;
    for (const auto &object : objects) {
      if (object->hit(r, interval(ray_dist.min_lim, closest), temp_rec)) {
        hit_anything = true;
        closest = temp_rec.t;
        rec = temp_rec;
      }
    }
    return hit_anything;
  }
};
#endif // !HITTABLE_LIST_H
