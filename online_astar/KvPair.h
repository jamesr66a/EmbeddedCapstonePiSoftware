#ifndef _KVPAIR_H_
#define _KVPAIR_H_

#include <cstdlib>
#include <functional>

namespace online_astar {

template <typename KeyType, typename ValueType> struct KvPair {
  KvPair(KeyType k, ValueType v) : k(k), v(v) {}

  KeyType k;
  ValueType v;
};

} // namespace online_astar

namespace std {

template <typename KeyType, typename ValueType>
struct hash<online_astar::KvPair<KeyType, ValueType> > {
public:
  size_t
  operator()(const online_astar::KvPair<KeyType, ValueType> &item) const {
    return std::hash<KeyType>()(item.k);
  }
};

} // namespace std

#endif /* _KVPAIR_H_ */
