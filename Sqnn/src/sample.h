//
// Created by cj on 8-4-24.
//

#ifndef KDE_SUB_QUADRATIC_SAMPLE_H
#define KDE_SUB_QUADRATIC_SAMPLE_H

#include <vector>
#include <Eigen/Dense>
#include "KDE.h"

inline void degreeWeight(KDE *kde, float *out){
	const float ownContribution = 1; //TODO make this calculated based on kernel.
	const Eigen::MatrixXf *data = kde->getDataRef();
	for(int i = 0; i < data->rows(); ++i){
		out[i] = kde->query(data->col(i)) - ownContribution;
	}
}

#endif //KDE_SUB_QUADRATIC_SAMPLE_H
