//
// Created by cj on 3-5-24.
//

#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "IoOperation.h"
#include "geometric.h"
#include "kernel_function.h"

class SpeedTest : public ::testing::Test {
protected:
  // You can define the constructor and destructor:

  SpeedTest() {
    int n, d;
    int k = 10, m = 500, trees = 10;
    double sigma = 3.3366;
    float rho, h;
    std::string _filename = ("../../Sqnn/data/aloi-clean.data");
    std::cout << _filename << "\n";
    io::LoadData(_filename, n, d, rho, h, data);
    this->kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);
    this->dist = data.colwise()-((Eigen::VectorXf) data.col(0));
    this->dist.colwise().norm();
  }

  ~SpeedTest() override = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

public:
  Eigen::MatrixXf data;
  kernel::kernelLambda<float> kernel;
  Eigen::MatrixXf dist;

};

TEST_F(SpeedTest, SetupOverheadTimeAprox){}

TEST_F(SpeedTest, ForLoop) {
  Geometric::DistanceSecondNorm(data, data.col(0));
}

TEST_F(SpeedTest, EigenDist){
  Eigen::VectorXf point = data.col(0);
  (data.colwise()-point).colwise().norm();
}

TEST_F(SpeedTest, KernelCost){
  for(unsigned long i = 0; i < this->dist.size(); ++i)
    this->kernel(this->dist(i));
}