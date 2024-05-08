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
//    float sum = 0;
//    std::vector<float> distances = Geometric::DistanceSecondNorm(data, q);
//    for (float &distance: distances)
//      sum += (*kernel)(distance);
//    return sum / (float) n;

    auto distances = (data.colwise() - q).colwise().lpNorm<2>();
    float sum = 0;
    for (int i = 0; i < distances.size(); ++i)
      sum += (*kernel)(distances(i));
    return sum / (float) data.size();
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
