//
// Created by cj on 28-4-24.
//

#include <Eigen/Dense>
#include <gtest/gtest.h>
#include "geometric.h"


TEST(GeoTest_distance_Test, distance_squared_matrix) {
  Eigen::MatrixXf data(3, 3);
  data << 1, 2, 3,
      4, 5, 6,
      7, 8, 9;
  Eigen::VectorXf point(3);
  point << 5, 5, 5;

  std::vector<float> dist = Geometric::DistanceSecondNorm(data, point);
  std::vector<float> exp_dist{21, 18, 21};

  EXPECT_EQ(dist.size(), exp_dist.size());

  for (int i = 0; i < dist.size(); ++i) {
    EXPECT_EQ(dist[i], exp_dist[i]);
  }
}

TEST(GeoTest_distance_Test, distance_wide_matrix) {
  Eigen::MatrixXf data(3, 6);
  data << 1, 2, 3, 3, 2, 1,
      4, 5, 6, 6, 5, 4,
      7, 8, 9, 9, 8, 7;
  Eigen::VectorXf point(3);
  point << 5, 5, 5;

  std::vector<float> dist = Geometric::DistanceSecondNorm(data, point);
  std::vector<float> exp_dist{21, 18, 21, 21, 18, 21};

  EXPECT_EQ(dist.size(), exp_dist.size());

  for (int i = 0; i < dist.size(); ++i) {
    EXPECT_EQ(dist[i], exp_dist[i]);
  }
}

TEST(GeoTest_distance_Test, distance_tall_matrix) {
  Eigen::MatrixXf data(3, 3);
  data << 1, 2, 3,
      4, 5, 6,
      7, 8, 9;
  Eigen::VectorXf point(3);
  point << 5, 5, 5;

  std::vector<float> dist = Geometric::DistanceSecondNorm(data, point);
  std::vector<float> exp_dist{21, 18, 21};

  EXPECT_EQ(dist.size(), exp_dist.size());

  for (int i = 0; i < dist.size(); ++i) {
    EXPECT_EQ(dist[i], exp_dist[i]);
  }
}

TEST(GeoTest_distance_Test, input_modify) {
  Eigen::MatrixXf data(3, 3);
  data << 1, 2, 3,
      4, 5, 6,
      7, 8, 9;
  Eigen::VectorXf point(3);
  point << 5, 5, 5;

  Eigen::MatrixXf data_base = data.block(0, 0, 3, 3);

  std::vector<float> dist = Geometric::DistanceSecondNorm(data, point);

  for (int i = 0; i < data.size(); ++i) {
    EXPECT_EQ(data(i / 3, i % 3), data_base(i / 3, i % 3));
  }
}