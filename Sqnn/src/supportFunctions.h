//
// Created by cj on 3-4-24.
//

#ifndef KDE_SUB_QUADRATIC_SUPPORTFUNCTIONS_H
#define KDE_SUB_QUADRATIC_SUPPORTFUNCTIONS_H

#include "kernelFunction.h"

namespace support {

	inline float computeTao(const Eigen::MatrixXf &x, const kernel::kernelLambda<float> &kernel) {
		assert(x.rows() > 0 && x.cols() > 0);

		float tao = 1;
		printf("cols:%ld\n", x.cols());
		printf("rows:%ld\n", x.rows());

#pragma omp parallel for reduction(min:tao) default(shared)
			for (int i = 0; i < x.cols(); ++i) {
				if (i % 1000 == 0) printf("process: %d\n", i);
				for (int j = i + 1; j < x.cols(); ++j) {
					tao = std::min(kernel(x.col(i), x.col(j)), tao);
				}
			}
		return tao;
	}

	//setting private
	namespace privates {
		inline Eigen::VectorXf sigmas(const Eigen::MatrixXf &x) {
			Eigen::VectorXf means = x.rowwise().mean();
			Eigen::MatrixXf squared_diff = (x.array().colwise() - means.array()).pow(2);
			Eigen::VectorXf variances = squared_diff.rowwise().mean();
			Eigen::VectorXf sd = variances.array().sqrt();
			return sd;
		}
	}

	inline double sigmaExtreme(const Eigen::MatrixXf &x) {
		return privates::sigmas(x).maxCoeff();
	}

	inline double sigmaAverage(const Eigen::MatrixXf &x) {
		return privates::sigmas(x).mean();
	}
}

#endif //KDE_SUB_QUADRATIC_SUPPORTFUNCTIONS_H
