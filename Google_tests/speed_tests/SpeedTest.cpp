//
// Created by cj on 3-5-24.
//

#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <cmath>

#include "IoOperation.hpp"
#include "kernel_function.hpp"
#include "KdeNaive.hpp"
#include "KdeUsingMrpt.hpp"
#include "KdeSampling.hpp"
#include "sample.hpp"

std::string filename;
int k, m, trees;
float sigma;

class SpeedTest : public ::testing::Test {
protected:
  // You can define the constructor and destructor:

  SpeedTest() {
    int n, d;
    if (!io::LoadData(filename, n, d, data)) exit(-1);
    Sample::ShuffleMatrixRows(data); //complete random
    //Sample::ShuffleMatrixRows(data, 0); //set seed
    this->kernel = kernel::type::Gaussian;
    printf("sigma: %f\n\n", sigma);

    if (k == 0)
      kde = new KdeSampling(data, m, sigma, this->kernel);
    else
      kde = new KdeUsingMrpt(data, k, m, sigma, trees, this->kernel);

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
  KdeNaive KDE(data, kernel, sigma);
  auto start_time = std::chrono::high_resolution_clock::now();
  KDE.query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  printf("time KDE Naive: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
}

TEST_F(SpeedTest, Kde) {
  auto start_time = std::chrono::high_resolution_clock::now();
  auto result = this->kde->query(data.col(0));
  auto end_time = std::chrono::high_resolution_clock::now();
  auto actual_result = KdeNaive(data, kernel, sigma).query(data.col(100000));
  printf("time KDE Sampling only: %ld ns\n",
         std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count());
  printf("approx value: %f\n", result);
  printf("actual value: %f\n", actual_result);
  printf("off factor: %f\n", std::abs(actual_result - result) / actual_result);
}

TEST_F(SpeedTest, RemovingNearestNeighborFromSample_BeforeSampling) {
  const int number_of_samples = 100;
  Eigen::VectorXf kernel_distances_reference = (data.leftCols(number_of_samples).colwise() - data.col(0))
      .colwise()
      .lpNorm<2>();
  kernel_distances_reference = kernel::KernelFunction(kernel, sigma, kernel_distances_reference);

  std::vector<int> nearest_neighbor_index{0, 7, 3, 101};
  int elementsToRemove = 3;

  //TODO: start timing here
  auto start_time = std::chrono::high_resolution_clock::now();
  std::sort(nearest_neighbor_index.begin(), nearest_neighbor_index.end());
  Eigen::MatrixXf samples = data.block(0, 0, data.rows(), number_of_samples - elementsToRemove);
  for (int i = 0; i < elementsToRemove; ++i) {
    int current_col = nearest_neighbor_index[i];
    samples.block(0, current_col, data.rows(), (number_of_samples - elementsToRemove) - current_col - 1)
        = data.block(0, current_col + 1, data.rows(), (number_of_samples - elementsToRemove) - current_col - 1);
  }

  //compute the distance of the new block
  Eigen::VectorXf kernel_distances_real = (samples.colwise() - data.col(0)).colwise().lpNorm<2>();
  kernel_distances_real = kernel::KernelFunction(kernel, sigma, kernel_distances_real);
  //TODO: end timing
  auto end_time = std::chrono::high_resolution_clock::now();
  auto time_spent = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();


  Eigen::VectorXf removed_elements_kernel_contribution(elementsToRemove);
  for (int i = 0; i < elementsToRemove; ++i) {
    removed_elements_kernel_contribution(i) = (data.col(nearest_neighbor_index[i]) - data.col(0)).lpNorm<2>();
  }
  removed_elements_kernel_contribution = kernel::KernelFunction(kernel, sigma, removed_elements_kernel_contribution);
  float removed_elements_contribution = removed_elements_kernel_contribution.sum();

  printf("expected value: %f\n", kernel_distances_reference.sum());
  printf("actual value before subtraction: %f\n", kernel_distances_real.sum());
  printf("actual value: %f\n", kernel_distances_real.sum() + removed_elements_contribution);
  printf("time spent: %ld ns\n\n", time_spent);
}

TEST_F(SpeedTest, RemovingNearestNeighborFromSample_AfterSampling) {
  const int number_of_samples = 100;
  Eigen::VectorXf kernel_distances_reference = (data.leftCols(number_of_samples).colwise() - data.col(0))
      .colwise()
      .lpNorm<2>();
  kernel_distances_reference = kernel::KernelFunction(kernel, sigma, kernel_distances_reference);

  std::vector<int> nearest_neighbor_index{0, 7, 3, 101};
  int elementsToRemove = 3;

  auto predicate = [number_of_samples](int x){return x > number_of_samples;};

  //TODO: start timing here
  auto start_time = std::chrono::high_resolution_clock::now();

  Eigen::VectorXf kernel_distances = (data.leftCols(number_of_samples).colwise() - data.col(0)).colwise().lpNorm<2>();
  kernel_distances = kernel::KernelFunction(kernel, sigma, kernel_distances);

  auto iterator = std::remove_if(nearest_neighbor_index.begin(), nearest_neighbor_index.end(), predicate);
  for (auto d = nearest_neighbor_index.begin(); d != iterator; ++d)
    kernel_distances(*d) = 0;
    //kernel_distances(*d) = & 0;

  //TODO: end timing
  auto end_time = std::chrono::high_resolution_clock::now();
  auto time_spent = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();


  Eigen::VectorXf removed_elements_kernel_contribution(elementsToRemove);
  for (int i = 0; i < elementsToRemove; ++i) {
    removed_elements_kernel_contribution(i) = (data.col(nearest_neighbor_index[i]) - data.col(0)).lpNorm<2>();
  }
  removed_elements_kernel_contribution = kernel::KernelFunction(kernel, sigma, removed_elements_kernel_contribution);
  float removed_elements_contribution = removed_elements_kernel_contribution.sum();

  printf("expected value: %f\n", kernel_distances_reference.sum());
  printf("actual value before subtraction: %f\n", kernel_distances.sum());
  printf("actual value: %f\n", kernel_distances.sum() + removed_elements_contribution);
  printf("time spent: %ld ns\n\n", time_spent);
}

int main(int argc, char **argv) {
  if (argc != 6) {
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