//
// Created by cj on 10-4-24.
//

#ifndef KDE_SUB_QUADRATIC_CONTROLLER_H
#define KDE_SUB_QUADRATIC_CONTROLLER_H

#include <Eigen/Dense>

#include "kernelFunction.h"
#include "KdeUsingMrpt.h"
#include "KdeNaive.h"

inline void runCppStyle(const int vertices, const int dimensions, const int nearestNeighbor,
												const int samples, const int trees, const std::pair<int,int> rhoPair,const double sigma) {
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);

	//setup data
	const Eigen::MatrixXf data(dimensions, vertices);

	//compute rho.
	//TODO: actually use
	const float rho = kernel(data.col(rhoPair.first), data.col(rhoPair.second));

	//TODO: multi-level KDE instead
	const KdeUsingMrpt kde(data, vertices, nearestNeighbor, samples, trees, &kernel);
	//TODO: Remove the Naive, only for testing.
	const KdeNaive kdeNaive(data, vertices, &kernel);
}

#endif //KDE_SUB_QUADRATIC_CONTROLLER_H
