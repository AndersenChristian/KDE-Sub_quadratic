//
// Created by cj on 28-5-24.
//

#include <gtest/gtest.h>

#include "Alias.hpp"


TEST(AliasTest, test1){
  std::vector<float> data{1,2,3,4};
  Alias<float, decltype(data.size())> alias(data.data(), data.size());
  std::vector<int> samples(4);
  for(int i = 0; i < 100000; ++i)
    samples[alias.Sample()]++;
  for(int i: samples){
    std::cout << i << "\n";
  }
}
