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
#include "geometric.h"

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
    std::vector<int> ann_list(N_);
    int numberOfCandidates = 0;
    mrpt_.query(q, ann_list.data(), &numberOfCandidates);

    //compute NN contribution,
    Eigen::MatrixXf nnMatrix = SubMatrixFromIndexes(ann_list);
    float sum_a = 0;
    for (float f: Geometric::DistanceSecondNorm(nnMatrix, q))
      sum_a += (*KERNEL_)(f);
    sum_a /= (float) numberOfCandidates;

    //compute sample contribution
    float sum_b = 0;
    std::vector<float> distances = Geometric::DistanceSecondNorm(DATA_.leftCols(SAMPLES_ + KNN_ + 1), q);
    int count_samples = 0, index = 0;
    while (count_samples < SAMPLES_) {
      if (std::find(ann_list.begin(), ann_list.end(), index++) != ann_list.end()) continue;
      ++count_samples;
      sum_b += (*KERNEL_)(distances[index - 1]);
    }
    sum_b /= (float) SAMPLES_;

    //compute total approx
    return (float) numberOfCandidates / (float) N_ * sum_a + (float) (N_ - numberOfCandidates) / (float) N_ * sum_b;

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
    Eigen::MatrixXf out(DATA_.rows(), indexes.size());
    for (int i = 0; i < (int) indexes.size(); ++i) {
      int colIndex = indexes[i];
      // Extract the column using Eigen's col method
      out.col(i) = DATA_.col(colIndex);
    }
    return out;
  }

};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
