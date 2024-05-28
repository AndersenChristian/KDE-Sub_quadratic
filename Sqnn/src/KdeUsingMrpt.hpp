//
// Created by cj on 26-3-24.
//

#ifndef KDE_SUB_QUADRATIC_KDEUSINGMRPT_HPP
#define KDE_SUB_QUADRATIC_KDEUSINGMRPT_HPP

#include <utility>
#include <vector>
#include <unordered_set>

#include "Mrpt.hpp"
#include "kernel_function.hpp"
#include "KDE.hpp"


class KdeUsingMrpt : public KDE {
public:
  //TODO Should only handle allocation. Make method for isValid after.
  KdeUsingMrpt(const Eigen::MatrixXf &data, int k, int samples, float sigma, int trees, // NOLINT(*-msc51-cpp)
               kernel::type kernel)
      : KNN_(k), N_((int) data.cols()), SAMPLES_(samples), SIGMA_(sigma), DATA_(data), KERNEL_(kernel),
        mrpt_(data) {
    //if no k is needed (low accuracy)
    //TODO: make second constructor since mrpt is still setup.
    if (k == 0) return;

    //Needed for the ANN to be setup.
    mrpt_.grow_autotune(TARGET_RECALL_, KNN_, trees);
    if (!mrpt_.is_autotuned()) printf("couldn't auto-tune!\n\n");
  }


  float query(const Eigen::VectorXf &q) override {
    std::vector<int> ann_list(0);
    float sum_a = 0;
    if (KNN_ != 0) {
      ann_list.resize(KNN_);
      sum_a = NNContribution(q, ann_list) / (float) KNN_;
    }

    float sum_b = SampleContribution(q, ann_list);

    //compute total approx
    sum_a = ((float) KNN_ / (float) N_) * sum_a;
    sum_b = ((float) (N_ - KNN_) / (float) N_) * sum_b;
    return sum_a + sum_b;

    //return 0;


  }

  const Eigen::MatrixXf &getData() override {
    return DATA_;
  }

  ~KdeUsingMrpt() override = default;

private:
  const int KNN_, N_, SAMPLES_;
  const double TARGET_RECALL_ = 0.5;
  const float SIGMA_;
  const Eigen::MatrixXf DATA_;
  const kernel::type KERNEL_;
  Mrpt mrpt_;

  inline Eigen::MatrixXf SubMatrixFromIndexes(std::vector<int> &indexes) {
    Eigen::MatrixXf out(DATA_.rows(), KNN_);
    for (int i = 0; i < (int) KNN_; ++i) {
      int colIndex = indexes[i];
      out.col(i) = DATA_.col(colIndex);
    }
    return out;
  }

  inline float NNContribution(const Eigen::VectorXf &q, std::vector<int> &ann_list) {
    mrpt_.query(q, ann_list.data());
    Eigen::MatrixXf nnMatrix = SubMatrixFromIndexes(ann_list);
    Eigen::VectorXf distance_a = (nnMatrix.colwise() - q).colwise().lpNorm<2>();
    distance_a = kernel::KernelFunction(KERNEL_, SIGMA_, distance_a);
    float sum = distance_a.sum();
    return sum;
  }

  inline float SampleContribution(const Eigen::VectorXf &q, std::vector<int> &ann_list) {
    const int COLS = KNN_ + SAMPLES_;
    auto predicate = [COLS](int x) { return x > COLS; };
    auto iterator = std::remove_if(ann_list.begin(), ann_list.end(), predicate);
    Eigen::VectorXf distance_b = (DATA_.leftCols(COLS).colwise() - q).colwise().lpNorm<2>();
    distance_b = kernel::KernelFunction(KERNEL_, SIGMA_, distance_b);

    //remove the nearest neighbor contribution
    int count = 0;
    for (auto d = ann_list.begin(); d != iterator; ++d) {
      count++;
      distance_b[*d] = 0;
    }

    return distance_b.sum() / (float) (COLS - count);
  }

};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_HPP
