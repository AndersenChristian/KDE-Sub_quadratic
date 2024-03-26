//
// Created by cj on 26-3-24.
//

#ifndef KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
#define KDE_SUB_QUADRATIC_KDEUSINGMRPT_H

#include <ANN/Mrpt.h>

class KDE {
	//TODO implementation
	//Should only handle allocation. Make method for isValid after.
	KDE(const Eigen::MatrixXf data, int n, int d, int samples, float sigma)
			: data(data), n(n), d(d), samples(samples), sigma(sigma) {}

	


private:
	Eigen::MatrixXf data;
	int n, d, samples;
	float sigma;
};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
