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
  KdeNaive(const Eigen::MatrixXf &data, kernel::kernelLambda<float> *kernel)
      : data(data), kernel(kernel), n((int) data.rows()) {}

  float query(const Eigen::VectorXf &q) override {
    float sum = 0;
#pragma omp parallel for reduction(+:sum) shared(q) default(none)
    for (int i = 0; i < n; ++i) {
      sum += (*kernel)(data.col(i), q);
    }
    return sum / (float) n;
  }

  const Eigen::MatrixXf &getData() override {
    return data;
  }

  ~KdeNaive() override = default;

private:
  const Eigen::MatrixXf &data;
  const kernel::kernelLambda<float> *kernel;
  const int n;
};

#endif //KDE_SUB_QUADRATIC_KDENAIVE_H
