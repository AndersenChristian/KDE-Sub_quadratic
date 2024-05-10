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
#include "KdeSampling.h"
#include "sample.h"

std::string filename;
int k, m, trees;
float sigma;

class SpeedTest : public ::testing::Test {
protected:
  // You can define the constructor and destructor:

  SpeedTest() {
    int n, d;
    if (!io::LoadData(filename, n, d, data)) exit(-1);
    Sample::ShuffleMatrixRows(data);
    this->kernel = kernel::type::Gaussian;
    printf("sigma: %f\n\n", sigma);

    if(k == 0)
      kde = new KdeSampling(data, m, sigma, this-> kernel);
    else
      kde = new KdeUsingMrpt(data, k, m, trees, this -> kernel);

    //processor limitation for the sake of fair testing
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
  kernel::type kernel;
  KDE *kde;
};

TEST_F(SpeedTest, NaiveKDE) {
  KdeNaive KDE(data, kernel::type::Gaussian, sigma);
  auto start_time = std::chrono::high_resolution_clock::now();
  KDE.query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  printf("time KDE Naive: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}

TEST_F(SpeedTest, Kde){
  auto start_time = std::chrono::high_resolution_clock::now();
  auto result = this->kde->query(data.col(100000));
  auto end_time = std::chrono::high_resolution_clock::now();
  auto actual_result = KdeNaive(data, kernel::type::Gaussian, sigma).query(data.col(100000));
  printf("time KDE Sampling only: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
  printf("approx value: %f\n", result);
  printf("actual value: %f\n", actual_result);
  printf("off factor: %f\n", std::abs(actual_result-result) / actual_result);
}

int main(int argc, char **argv){
  if(argc != 6){
    printf("missing arguments");
    fflush(stdout);
    exit(-1);
  }
  k = std::stoi(argv[2]);
  m = std::stoi(argv[3]);
  trees = std::stoi(argv[4]);
  sigma = std::stof(argv[5]);

  testing::InitGoogleTest();

  //could be better
  filename = "Sqnn/data/" + std::string(argv[1]);

  return RUN_ALL_TESTS();
}