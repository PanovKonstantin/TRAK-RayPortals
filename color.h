#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

#include <iostream>
#include <ostream>

using color = vec3;

inline void write_color(std::ostream &out, color c, int sample_per_pixel) {
  auto scale = 1.0 / sample_per_pixel;
  color scaled = c * scale;
  static const interval intensity(.0, .999);

  out << static_cast<int>(256 * intensity.clamp(scaled.x())) << ' '
      << static_cast<int>(256 * intensity.clamp(scaled.y())) << ' '
      << static_cast<int>(256 * intensity.clamp(scaled.z())) << '\n';
}

#endif // !COLOR_H
