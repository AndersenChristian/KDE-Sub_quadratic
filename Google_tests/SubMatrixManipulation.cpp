//
// Created by cj on 20-5-24.
//

#include <gtest/gtest.h>
#include <Eigen/Dense>

TEST(Push, Push1) {
  Eigen::MatrixXf matrix_original(3, 3);
  matrix_original << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Eigen::MatrixXf matrix_manipulated(3, 2);

  std::cout << matrix_original << "\n\n";

  matrix_manipulated.block(0, 0, 3, 1) = matrix_original.block(0, 0, 3, 1);
  matrix_manipulated.block(0, 1, 3, 1) = matrix_original.block(0, 2, 3, 1);

  std::cout << matrix_manipulated << "\n\n\n";
}

TEST(REMOVE, removeIf) {
  std::vector<int> data{1, 2, 3, 4, 5, 6};
  std::vector<int> sum{1, 2, 3, 4, 5, 6};
  int val = 2;
  auto predicate = [val](int x) { return x > val; };
  auto iterator = std::remove_if(data.begin(), data.end(), predicate);
  for (auto d = data.begin(); d != iterator; ++d)
    sum[*d - 1] = 0;
  for (int i: sum)
    std::cout << i << "\n";
}

TEST(REMOVE, removeIf2) {
  Eigen::VectorXf kernel_distances(6);
  kernel_distances << 1, 2, 3, 4, 5, 6;
  std::vector<int> nearest_neighbor_index{0, 3, 7};
  int val = 6;
  auto predicate = [val](int x) { return x > val; };
  auto iterator = std::remove_if(nearest_neighbor_index.begin(), nearest_neighbor_index.end(), predicate);
  for (auto d = nearest_neighbor_index.begin(); d != iterator; ++d)
    kernel_distances(*d) = 0;
  std::cout << "\n\n" << kernel_distances;
}