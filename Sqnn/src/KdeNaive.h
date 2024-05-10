//
// Created by cj on 10-4-24.
//

#ifndef KDE_SUB_QUADRATIC_KDENAIVE_H
#define KDE_SUB_QUADRATIC_KDENAIVE_H

#include "Mrpt.h"
#include <Eigen/Dense>
#include <utility>
#include <vector>

#include "KDE.h"

class KdeNaive : public KDE {
public:
  KdeNaive(const Eigen::MatrixXf &data, kernel::type kernel, float sigma)
      : DATA_(data), KERNEL_(kernel), SIGMA_(sigma) {}

  float query(const Eigen::VectorXf &q) override {
    Eigen::VectorXf distances = (DATA_.colwise() - q).colwise().lpNorm<2>();
    distances = kernel::KernelFunction(KERNEL_, SIGMA_, distances);
    return distances.sum() / (float) DATA_.cols();
  }

  const Eigen::MatrixXf &getData() override {
    return DATA_;
  }

  ~KdeNaive() override = default;

private:
  const Eigen::MatrixXf &DATA_;
  const kernel::type KERNEL_;
  const float SIGMA_;
};

#endif //KDE_SUB_QUADRATIC_KDENAIVE_H
