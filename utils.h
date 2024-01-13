#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstdlib>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double pi = 3.1415926535897932385;

inline double deg_to_rad(double degrees) { return degrees * pi / 180.; }

inline double random_double() { return rand() / (RAND_MAX + 1.0); };

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
};

#endif // !UTILS_H
