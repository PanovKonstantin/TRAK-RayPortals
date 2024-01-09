#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>
const double infinity = std::numeric_limits<double>::infinity();

class interval {
public:
  double min_lim, max_lim;

  interval(double _min, double _max) : min_lim(_min), max_lim(_max) {}
  interval() : min_lim(+infinity), max_lim(-infinity) {}

  bool contains(double value) { return min_lim <= value && value <= max_lim; }
  bool surrounds(double value) { return min_lim < value && value < max_lim; }
  double clamp(double value) const {
    if (value < min_lim)
      return min_lim;
    if (value > max_lim)
      return max_lim;
    return value;
  };

  static const interval empty, univers;
};

const static interval empty(+infinity, -infinity);
const static interval univers(-infinity, +infinity);

#endif // !INTERVAL_H
