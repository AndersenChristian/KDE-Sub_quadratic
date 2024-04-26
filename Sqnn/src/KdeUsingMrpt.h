//
// Created by cj on 26-3-24.
//

#ifndef KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
#define KDE_SUB_QUADRATIC_KDEUSINGMRPT_H

#include "Mrpt.h"
#include <utility>
#include <vector>

#include "kernel_function.h"
#include "KDE.h"
#include "geometric.h"

class KdeUsingMrpt : public KDE {
public:
  //TODO Should only handle allocation. Make method for isValid after.
  KdeUsingMrpt(const Eigen::MatrixXf &data, int k, int samples, int trees, // NOLINT(*-msc51-cpp)
               kernel::kernelLambda<float> *kernel)
      : KNN_(k), DATA_(data), KERNEL_(kernel), mrpt_(data), n((int) data.cols()),
        samples(samples) {
    //this->data = data;
    //random number-generator setup
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    generator_.seed(seed);

    //if no k is needed (low accuracy)
    //TODO: make sekund constructor since mrpt is still setup.
    if (k == 0) return;

    //Needed for the ANN to be setup.
    mrpt_.grow_autotune(TARGET_RECALL_, KNN_, trees);
    if (!mrpt_.is_autotuned()) printf("couldn't autotune!\n\n");
  }


  float query(const Eigen::VectorXf &q) override {
    //NOTE: not sure if this is suppose to be here
    //if (KNN_ == 0) return randomSampleAndSum(q);

    std::vector<int> ann_list(n);
    int numberOfCandidates = 0;
    mrpt_.query(q, ann_list.data(), &numberOfCandidates);

    //compute NN contribution,
    Eigen::MatrixXf nnMatrix(DATA_.rows(), ann_list.size());
    for (int i = 0; i < (int) ann_list.size(); ++i) {
      int colIndex = ann_list[i];
      // Extract the column using Eigen's col method
      nnMatrix.col(i) = DATA_.col(colIndex);
    }
    float sum_a = 0;
    for (float f: geometric::DistanceSecondNorm(nnMatrix, q))
      sum_a += f;
    sum_a /= (float) numberOfCandidates;


    //compute sample contribution
    float sum_b = 0;
    int index;
#pragma omp parallel for private(index) reduction(+: sum_b) shared(numberOfCandidates, ann_list, q) default(none)
    for (int i = 0; i < samples; ++i) {
      do {
        index = randomIndex(n);
      } while (std::find(ann_list.begin(), ann_list.begin() + numberOfCandidates, index) !=
               ann_list.begin() + numberOfCandidates);
      sum_b += (*KERNEL_)(DATA_.col(index), q);
    }
    sum_b /= (float) samples;

    //compute total approx
    return (float) numberOfCandidates / (float) n * sum_a + (float) (n - numberOfCandidates) / (float) n * sum_b;

  }

  const Eigen::MatrixXf &getData() override {
    return DATA_;
  }

  ~KdeUsingMrpt() override = default;

private:
  const int KNN_;
  const double TARGET_RECALL_ = 0.9;
  const Eigen::MatrixXf DATA_;
  const kernel::kernelLambda<float> *KERNEL_;
  Mrpt mrpt_;
  std::mt19937 generator_;

  const int n, samples;

  int randomIndex(const int max) {
    std::uniform_int_distribution<int> distribution(0, max - 1);
    return distribution(this->generator_);
  }

  inline Eigen::MatrixXf SubMatrixFromIndexes(std::vector<int> indexes) {
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
