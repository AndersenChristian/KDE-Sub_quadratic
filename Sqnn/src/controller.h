//
// Created by cj on 10-4-24.
//

#ifndef KDE_SUB_QUADRATIC_CONTROLLER_H
#define KDE_SUB_QUADRATIC_CONTROLLER_H

#include <Eigen/Dense>

#include "kernelFunction.h"
#include "KdeUsingMrpt.h"
#include "KdeNaive.h"

inline void
runCppStyle(const Eigen::MatrixXf &data, const int vertices, const int dimensions, const int nearestNeighbor,
						const int samples, const int trees, const float rho, const double sigma) {
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);

//compute rho.
//TODO: actually use

//TODO: multi-level KDE instead
	const KdeUsingMrpt kde(data, vertices, nearestNeighbor, samples, trees, &kernel);
//TODO: Remove the Naive, only for testing.
	const KdeNaive kdeNaive(data, vertices, &kernel);
}

#endif //KDE_SUB_QUADRATIC_CONTROLLER_H
