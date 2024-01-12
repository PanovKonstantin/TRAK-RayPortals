#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

#include <cmath>
#include <iostream>
#include <ostream>

using color = vec3;

inline color linear_to_gamma(color linear) {
  return color(std::sqrt(linear.x()), std::sqrt(linear.y()),
               std::sqrt(linear.z()));
}

inline void write_color(std::ostream &out, color c, int sample_per_pixel) {
  auto scale = 1.0 / sample_per_pixel;
  color scaled = c * scale;
  color gamma_corrected = linear_to_gamma(scaled);

  static const interval intensity(.0, .999);
  out << static_cast<int>(256 * intensity.clamp(gamma_corrected.x())) << ' '
      << static_cast<int>(256 * intensity.clamp(gamma_corrected.y())) << ' '
      << static_cast<int>(256 * intensity.clamp(gamma_corrected.z())) << '\n';
}

#endif // !COLOR_H
