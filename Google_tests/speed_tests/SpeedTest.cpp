//
// Created by cj on 3-5-24.
//

#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "IoOperation.h"
#include "kernel_function.h"
#include "KdeNaive.h"
#include "KdeUsingMrpt.h"

class SpeedTest : public ::testing::Test {
protected:
  // You can define the constructor and destructor:

  SpeedTest() {
    int n, d;
    int k = 10, m = 500, trees = 10;
    sigma = 3.3366;
    float rho, h;
    std::string _filename = ("../../Sqnn/data/aloi-clean.data");
    io::LoadData(_filename, n, d, rho, h, data);
    this->kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);
    this->dist = (data.colwise() - ((Eigen::VectorXf) data.col(0))).colwise().norm();
    kDist = this->dist / sigma;
    omp_set_num_threads(1);
    Eigen::setNbThreads(1);
  }

  ~SpeedTest() override = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  void TearDown() override {
    std::cout << std::endl;
  }

public:
  Eigen::MatrixXf data;
  kernel::kernelLambda<float> kernel;
  Eigen::VectorXf dist;
  Eigen::VectorXf kDist;
  double sigma;

};

TEST_F(SpeedTest, EigenDist) {
  Eigen::VectorXf point = data.col(0);
  (data.colwise() - point).colwise().squaredNorm();
}

TEST_F(SpeedTest, KernelDivision) {
  this->dist / sigma;
}

TEST_F(SpeedTest, KernelCost) {
  for (unsigned long i = 0; i < this->dist.size(); ++i)
    this->kernel(this->dist((long) i));
}

TEST_F(SpeedTest, Distance) {
  Eigen::VectorXf point = data.col(0);
  auto start_time = std::chrono::high_resolution_clock::now();
  auto e = (data.colwise() - point).colwise().squaredNorm();
  auto end_time = std::chrono::high_resolution_clock::now();
  printf("time Eigen distance: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
  e(0);
}

TEST_F(SpeedTest, NaiveKDE) {
  KdeNaive KDE(data, &kernel);
  auto start_time = std::chrono::high_resolution_clock::now();
  auto f = KDE.query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  f += f;
  printf("time KDE Naive: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}

TEST_F(SpeedTest, AnnKDE) {
  KdeUsingMrpt KDE(data, 120, 430, 10, &kernel);
  auto start_time = std::chrono::high_resolution_clock::now();
  auto f = KDE.query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  f += f;
  printf("time KDE Naive: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}