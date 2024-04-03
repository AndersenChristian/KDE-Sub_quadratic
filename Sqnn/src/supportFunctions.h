//
// Created by cj on 3-4-24.
//

#ifndef KDE_SUB_QUADRATIC_SUPPORTFUNCTIONS_H
#define KDE_SUB_QUADRATIC_SUPPORTFUNCTIONS_H

//#include <Eigen/Core>
#include "kernelFunction.h"

inline float computeTao(const Eigen::MatrixXf &x, const double sigma,
												const kernel::kernelLambda<float> &kernel = kernel::kernel_function<float>(
														kernel::type::Gaussian)) {
	float tao = 1;
	for (int i = 0; i < x.cols(); ++i) {
		for (int j = i; j < x.cols(); ++j) {
			tao = std::min(kernel(x.row(i), x.row(j), sigma), tao);
		}
	}
	return tao;
}

inline Eigen::VectorXf sigmas(const Eigen::MatrixXf &x){
	Eigen::VectorXf means = x.colwise().mean();
	Eigen::MatrixXf squared_diff = (x.array().colwise() - means.array().pow(2));
	Eigen::VectorXf variances = squared_diff.rowwise().mean();
	Eigen::VectorXf sd = variances.array().sqrt();
	return sd;
}

inline double sigmaExtreme(const Eigen::MatrixXf &x){
	return sigmas(x).maxCoeff();
}

inline double sigmaAverage(const Eigen::MatrixXf &x){
	return sigmas(x).mean();
}


#endif //KDE_SUB_QUADRATIC_SUPPORTFUNCTIONS_H
