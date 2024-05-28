//
// Created by cj on 10-5-24.
//

#ifndef KDE_SUB_QUADRATIC_KDESAMPLING_HPP
#define KDE_SUB_QUADRATIC_KDESAMPLING_HPP

#include <utility>
#include <vector>
#include <chrono>

#include "Mrpt.hpp"
#include "kernel_function.hpp"
#include "KDE.hpp"


class KdeSampling : public KDE {
public:
  //TODO Should only handle allocation. Make method for isValid after.
  KdeSampling(const Eigen::MatrixXf &data, int samples, float sigma,
               kernel::type kernel)
      :  SAMPLES_(samples), SIGMA_(sigma), DATA_(data), KERNEL_(kernel){
  }


  float query(const Eigen::VectorXf &q) override {
    Eigen::VectorXf distance_b = (DATA_.leftCols(SAMPLES_).colwise() - q).colwise().lpNorm<2>();
    distance_b = kernel::KernelFunction(KERNEL_, SIGMA_, distance_b);
    return distance_b.sum() / (float) SAMPLES_;
  }

  const Eigen::MatrixXf &getData() override {
    return DATA_;
  }

  ~KdeSampling() override = default;

private:
  const int SAMPLES_;
  const float SIGMA_;
  const Eigen::MatrixXf &DATA_;
  const kernel::type KERNEL_;
};

#endif //KDE_SUB_QUADRATIC_KDESAMPLING_HPP
