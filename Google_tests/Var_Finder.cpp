//
// Created by cj on 8-5-24.
//

#include <memory>
#include <string>
#include <Eigen/Dense>

#include "IoOperation.hpp"
#include "KdeUsingMrpt2.hpp"
#include "kernel_function.hpp"
#include "KdeNaive.hpp"
#include "sample.hpp"

Eigen::MatrixXf data;
std::string filename;
int n, d;
double sigma = 3.3366;
float rho, h;

bool CloseEnough(float approx, float actual, float max_deviation) {
  float tmp = std::abs(approx - actual);
  tmp /= actual;
  return tmp <= max_deviation;
}

std::vector<int> SetupCandidate() {
  const int SAMPLE_MAX = n - 1;
  const int K_MAX = std::ceil(std::sqrt(n));
  const int &TREES_MAX = K_MAX;
  const float MAX_DEVIATION = 0.01;

  std::vector<int> out(K_MAX / 10);
  int out_ptr = 0;
  kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);

  KdeNaive exactKDE(data, &kernel);
  const float EXACT_KDE_VALUE = exactKDE.query(data.col(0));
  std::unique_ptr<KdeUsingMrpt2> kde;


  for (int k = 0; k <= K_MAX; k += 10) {
    printf("k: %d\n", k);
    kde = std::make_unique<KdeUsingMrpt2>(data, k, 0, 20, sigma);
    printf("test\n");
    int start_index = 0, end_index = SAMPLE_MAX - k, middle;

    int safe_guard = 0;
    while (start_index != end_index) {
      middle = (end_index + start_index) / 2;
      kde->setSamples(middle);
      float tmp = kde->query(data.col(0));
      printf("test1\n");
      if (CloseEnough(tmp, EXACT_KDE_VALUE, MAX_DEVIATION))
        end_index = middle;
      else
        start_index = middle + 1;
      if (safe_guard++ == 20) {
        printf("s: %d\te: %d", start_index, end_index);
        exit(-1);
      }
    }
    printf("test2\n");
    out[out_ptr++] = start_index;
  }
  return out;
}

int main(int argc, char **argv) {
  if (argc != 2) exit(-1);
  filename = "Sqnn/data/" + std::string(argv[1]);
  if (!io::LoadData(filename, n, d, rho, h, data)) exit(-2);
  Sample::ShuffleMatrixRows(data);

  auto candidates = SetupCandidate();
  for (int i = 0; i < candidates.size(); ++i) {
    printf("knn: %d\tsample: %d\n", i, candidates[i]);
  }
}