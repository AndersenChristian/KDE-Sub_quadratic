//
// Created by cj on 26-3-24.
//

#ifndef KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
#define KDE_SUB_QUADRATIC_KDEUSINGMRPT_H

#include <utility>
#include <vector>
#include <unordered_set>

#include "Mrpt.h"
#include "kernel_function.h"
#include "KDE.h"


class KdeUsingMrpt : public KDE {
public:
  //TODO Should only handle allocation. Make method for isValid after.
  KdeUsingMrpt(const Eigen::MatrixXf &data, int k, int samples, int trees, // NOLINT(*-msc51-cpp)
               kernel::kernelLambda<float> *kernel)
      : KNN_(k), N_((int) data.cols()), SAMPLES_(samples), DATA_(data), KERNEL_(kernel),
        mrpt_(data) {
    //this->data = data;
    //random number-generator setup
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    generator_.seed(seed);

    //if no k is needed (low accuracy)
    //TODO: make second constructor since mrpt is still setup.
    if (k == 0) return;

    //Needed for the ANN to be setup.
    mrpt_.grow_autotune(TARGET_RECALL_, KNN_, trees);
    if (!mrpt_.is_autotuned()) printf("couldn't auto-tune!\n\n");
  }


  float query(const Eigen::VectorXf &q) override {
    float sum_a = 0;
    if (KNN_ != 0)
      sum_a = NNContribution(q) / (float) KNN_;

    float sum_b = SampleContribution(q) / (float) SAMPLES_;

    //compute total approx
    return ((float) KNN_ / (float) N_) * sum_a + ((float) (N_ - KNN_) / (float) N_) * sum_b;


  }

  const Eigen::MatrixXf &getData() override {
    return DATA_;
  }

  ~KdeUsingMrpt() override = default;

private:
  const int KNN_, N_, SAMPLES_;
  const double TARGET_RECALL_ = 0.9;
  const Eigen::MatrixXf DATA_;
  const kernel::kernelLambda<float> *KERNEL_;
  Mrpt mrpt_;
  std::mt19937 generator_;

  inline Eigen::MatrixXf SubMatrixFromIndexes(std::vector<int> &indexes) {
    Eigen::MatrixXf out(DATA_.rows(), KNN_);
    for (int i = 0; i < (int) KNN_; ++i) {
      int colIndex = indexes[i];
      // Extract the column using Eigen's col method
      out.col(i) = DATA_.col(colIndex);
    }
    return out;
  }

  inline float NNContribution(const Eigen::VectorXf &q) {
    std::vector<int> ann_list(N_);
    int numberOfCandidates = 0;
    auto pre_mrpt = std::chrono::high_resolution_clock::now();
    mrpt_.query(q, ann_list.data(), &numberOfCandidates);
    auto post_mrpt = std::chrono::high_resolution_clock::now();

    Eigen::MatrixXf nnMatrix = SubMatrixFromIndexes(ann_list);
    float sum = 0;
    auto distance_a = (nnMatrix.colwise() - q).colwise().lpNorm<2>();
    for (int i = 0; i < KNN_; ++i) sum += (*KERNEL_)(distance_a(i));
    auto post_nn_contribution = std::chrono::high_resolution_clock::now();

    //TEST prints
    printf("mrpt: %ld ns\n", std::chrono::duration_cast<std::chrono::nanoseconds>(post_mrpt - pre_mrpt).count());
    printf("nn contribution: %ld ns\n",
           std::chrono::duration_cast<std::chrono::nanoseconds>(post_nn_contribution - post_mrpt).count());

    return sum;
  }

  inline float SampleContribution(const Eigen::VectorXf &q) {
    auto pre_sample = std::chrono::high_resolution_clock::now();
    //compute sample contribution
    float sum = 0;
    auto distance_b = (DATA_.leftCols(SAMPLES_).colwise() - q).colwise().lpNorm<2>();
    for (int i = 0; i < SAMPLES_; ++i) sum += (*KERNEL_)(distance_b(i));
    auto post_sample = std::chrono::high_resolution_clock::now();

    printf("sample: %ld ns\n",
           std::chrono::duration_cast<std::chrono::nanoseconds>(post_sample - pre_sample).count());

    return sum;
  }

};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
