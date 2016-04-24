#include "node.h"

#include <cmath>

namespace online_astar {

#define LATTICE_LENGTH (15)

float Node::euclidean_distance(int32_t other_x, int32_t other_y) {
  using std::sqrt;
  int x_norm = x - other_x;
  int y_norm = y - other_y;
  return sqrt(x_norm * x_norm + y_norm * y_norm);
}

std::vector<std::tuple<int32_t, int32_t> >
Node::successors(const std::tuple<int32_t, int32_t> &x_bounds,
                 const std::tuple<int32_t, int32_t> &y_bounds) const {

  std::vector<std::tuple<int32_t, int32_t> > retval = {
    std::make_tuple(x + LATTICE_LENGTH, y),
    std::make_tuple(x - LATTICE_LENGTH, y),
    std::make_tuple(x, y + LATTICE_LENGTH),
    std::make_tuple(x, y - LATTICE_LENGTH)
  };

  retval.erase(std::remove_if(begin(retval), end(retval),
                              [&](const std::tuple<int32_t, int32_t> &in) {
                                auto x_local = std::get<0>(in);
                                auto y_local = std::get<1>(in);
                                bool rem = x_local < std::get<0>(x_bounds) ||
                                           x_local > std::get<1>(x_bounds) ||
                                           y_local < std::get<0>(y_bounds) ||
                                           y_local > std::get<1>(y_bounds);
                                return rem;
                              }),
               end(retval));

  return retval;
}

} // namespace online_astar
