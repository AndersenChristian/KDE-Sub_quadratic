//
// Created by cj on 3-5-24.
//

#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <cmath>
#include "IoOperation.h"
#include "kernel_function.h"
#include "KdeNaive.h"
#include "KdeUsingMrpt.h"
#include "KdeUsingMrpt2.h"
#include "KdeSampling.h"
#include "sample.h"

std::string filename;
int k = 0, m = 170, trees = 10;

class SpeedTest : public ::testing::Test {
protected:
  // You can define the constructor and destructor:

  SpeedTest() {
    int n, d;
    sigma = 3.3366;
    float rho, h;
    if (!io::LoadData(filename, n, d, rho, h, data)) exit(-1);
    this->kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);
    Sample::ShuffleMatrixRows(data);
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
  float sigma;
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
  KdeNaive KDE(data, kernel::type::Gaussian, sigma);
  auto start_time = std::chrono::high_resolution_clock::now();
  KDE.query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  printf("time KDE Naive: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}

TEST_F(SpeedTest, AnnKDE) {
  KdeUsingMrpt KDE(data, 0, 2100, 10, &kernel);
  auto start_time = std::chrono::high_resolution_clock::now();
  KDE.query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  printf("time Ann: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}

TEST_F(SpeedTest, MathSelf) {
  Eigen::VectorXf distances = (data.colwise() - data.col(0)).colwise().lpNorm<2>();
  auto pre_expr = std::chrono::high_resolution_clock::now();
  for(auto &d : distances){
    d = (kernel)(d);
  }
  auto post_expr = std::chrono::high_resolution_clock::now();
  printf("mat self - expr time: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(post_expr - pre_expr).count());
}

TEST_F(SpeedTest, MathEigen) {
  Eigen::VectorXf distances = (data.colwise() - data.col(0)).colwise().lpNorm<2>();
  auto pre_dist = std::chrono::high_resolution_clock::now();
  distances.operator/=(2);
  auto post_dist = std::chrono::high_resolution_clock::now();
  distances.unaryExpr([](float v){return std::exp(-v);});
  auto post_expr = std::chrono::high_resolution_clock::now();
  printf("division: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(post_dist - pre_dist).count());
  printf("expr: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(post_expr - post_dist).count());
}

TEST_F(SpeedTest, MathEigen2) {
  Eigen::VectorXf distances = (data.colwise() - data.col(0)).colwise().lpNorm<2>();
  float sigma = 2;
  auto pre_expr = std::chrono::high_resolution_clock::now();
  distances.unaryExpr([sigma](float v){return std::exp(-v/sigma);});
  auto post_expr = std::chrono::high_resolution_clock::now();
  printf("expr: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(post_expr - pre_expr).count());
}

TEST_F(SpeedTest, MathEigen_Using_Kernel) {
  const kernel::kernelLambda<float> _kf = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);
  Eigen::VectorXf distances = (data.colwise() - data.col(0)).colwise().lpNorm<2>();
  auto pre_expr = std::chrono::high_resolution_clock::now();
  distances.unaryExpr(_kf);
  auto post_expr = std::chrono::high_resolution_clock::now();
  printf("expr: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(post_expr - pre_expr).count());
}

TEST_F(SpeedTest, MathEigen_Using_Switch) {
  Eigen::VectorXf distances = (data.colwise() - data.col(0)).colwise().lpNorm<2>();
  auto pre_expr = std::chrono::high_resolution_clock::now();
  kernel::KernelFunction(kernel::type::Gaussian, (float) sigma, distances);
  auto post_expr = std::chrono::high_resolution_clock::now();
  printf("expr: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(post_expr - pre_expr).count());
}

TEST_F(SpeedTest, FastestOfAll){
  KdeUsingMrpt2 KDE(data, 120, 430, 10, sigma);
  auto start_time = std::chrono::high_resolution_clock::now();
  KDE.query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  printf("time KDE Naive: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}

TEST_F(SpeedTest, KdeSampling){
  KdeSampling kdeSampling(data, 3000, sigma, kernel::type::Gaussian);
  auto start_time = std::chrono::high_resolution_clock::now();
  auto result = kdeSampling.query(data.col(100000));
  auto end_time = std::chrono::high_resolution_clock::now();
  auto actual_result = KdeNaive(data, kernel::type::Gaussian, sigma).query(data.col(100000));
  printf("time KDE Sampling only: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
  printf("approx value: %f\n", result);
  printf("actual value: %f\n", actual_result);
  printf("off factor: %f\n", std::abs(actual_result-result) / actual_result);
}

int main(int argc, char **argv){
  if(argc != 5){
    printf("missing arguments");
    fflush(stdout);
    exit(-1);
  }
  testing::InitGoogleTest();

  k = std::stoi(argv[2]);
  m = std::stoi(argv[3]);
  trees = std::stoi(argv[4]);

  //could be better
  filename = "../../Sqnn/data/" + std::string(argv[1]);

  return RUN_ALL_TESTS();
}