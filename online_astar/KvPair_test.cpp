#include "KvPair.h"

#include <functional>
#include <gtest/gtest.h>

using online_astar::KvPair;

struct MyWeirdStruct {
  uint32_t a;
  uint32_t b;
  std::string meme;
};

TEST(KvPairTest, TestKvPairConstruction) {
  MyWeirdStruct test_item{ 3, 4, "test" };
  KvPair<std::string, MyWeirdStruct> kv("item1", test_item);
  EXPECT_EQ(kv.k, "item1");
  EXPECT_EQ(kv.v.a, test_item.a);
  EXPECT_EQ(kv.v.b, test_item.b);
  EXPECT_EQ(kv.v.meme, test_item.meme);
}

TEST(KvPairTest, TestHashFn) {
  KvPair<std::string, MyWeirdStruct> kv("item1", { 3, 4, "test" });
  auto kv_hash = std::hash<KvPair<std::string, MyWeirdStruct> >()(kv);
  auto str_hash = std::hash<std::string>()("item1");
  EXPECT_EQ(kv_hash, str_hash);
}
