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
    std::vector<int> ann_list;
    float sum_a = 0;
    if (KNN_ != 0) {
      ann_list.resize(KNN_);
      sum_a = NNContribution(q, ann_list) / (float) KNN_;
    }

    float sum_b = SampleContribution(q, ann_list) / (float) SAMPLES_;

    //compute total approx
    return ((float) KNN_ / (float) N_) * sum_a + ((float) (N_ - KNN_) / (float) N_) * sum_b;


  }

  const Eigen::MatrixXf &getData() override {
    return DATA_;
  }

  ~KdeUsingMrpt() override = default;

private:
  const int KNN_, N_, SAMPLES_;
  const double TARGET_RECALL_ = 0.5;
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

  inline float NNContribution(const Eigen::VectorXf &q, std::vector<int> &ann_list) {
    mrpt_.query(q, ann_list.data());
    Eigen::MatrixXf nnMatrix = SubMatrixFromIndexes(ann_list);
    float sum = 0;
    Eigen::VectorXf distance_a = (nnMatrix.colwise() - q).colwise().lpNorm<2>();
    distance_a = distance_a.block(0, 0, 1, KNN_);
    distance_a = kernel::KernelFunction(kernel::type::Gaussian, 3.3366, distance_a);
    sum = distance_a.sum();

    return sum;
  }

  inline float SampleContribution(const Eigen::VectorXf &q, std::vector<int> ann_list) {
    //compute sample contribution
    Eigen::VectorXf distance_b = (DATA_.leftCols(SAMPLES_ + KNN_).colwise() - q).colwise().lpNorm<2>();

    //removes any points that are apart from the nearest neighbor calculation.
    if (KNN_ > 0) {
      for (int i = 0; i < KNN_; ++i) {
        printf("index: %d\n", ann_list[i]);
        if (ann_list[i] > KNN_ + SAMPLES_) continue;
        distance_b(i) = MAXFLOAT;
      }
    }

    distance_b = kernel::KernelFunction(kernel::type::Gaussian, 3.3366, distance_b);
    return distance_b.sum();
  }

};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
