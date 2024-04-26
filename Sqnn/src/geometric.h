//
// Created by cj on 26-4-24.
//

#ifndef KDE_SUB_QUADRATIC_GEOMETRIC_H
#define KDE_SUB_QUADRATIC_GEOMETRIC_H

#include <Eigen/Dense>
#include <cmath>

namespace geometric {

  /**
 *
 *
 * @param DATA row major matrix of d rows.
 * @param POINT vector of size d.
 * @return a list of distances
 */
  std::vector<float> DistanceSecondNorm(const Eigen::MatrixXf &DATA, const Eigen::VectorXf &POINT) {
    std::vector<float> out(DATA.rows());
    for (int i = 0; i < DATA.rows(); ++i) {
      const float *p_i = &POINT(i);
      float *out_i = &out[i];
      for (int j = 0; j < DATA.cols(); ++j) {
        *out_i += (float) std::pow((DATA(i, j) - *p_i), 2);
      }
    }
    return out;
  }

  std::vector<float> DistanceSecondNorm(const Eigen::VectorXf &DATA, const Eigen::VectorXf &POINT) {
    return DistanceSecondNorm((Eigen::MatrixXf) Eigen::Map<const Eigen::VectorXf>(DATA.data(), DATA.size(), 1), POINT);
  }

}

#endif //KDE_SUB_QUADRATIC_GEOMETRIC_H
