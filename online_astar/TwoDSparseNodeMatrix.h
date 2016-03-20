#ifndef _TWODSPARSENODEMATRIX_H_
#define _TWODSPARSENODEMATRIX_H_

#include "KvPair.h"
#include "node.h"

#include <memory>
#include <stdint.h>
#include <unordered_map>

namespace online_astar {

class TwoDSparseMatrix {
public:
  TwoDSparseMatrix() {}
  std::shared_ptr<Node> query(int32_t x, int32_t y);
  void insert(Node n);

private:
  std::unordered_map<KvPair<
      int32_t, std::unordered_map<KvPair<int32_t, Node> > > > sparse_matrix;
}; // class TwoDSparseMatrix

} // namespace online_astar

#endif /* _TWODSPARSENODEMATRIX_H_ */
