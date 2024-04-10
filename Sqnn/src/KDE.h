//
// Created by cj on 8-4-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_H
#define KDE_SUB_QUADRATIC_KDE_H

#include <Eigen/Dense>

#include "kernelFunction.h"

class KDE {
public:
	virtual float query(const Eigen::VectorXf &q) = 0;
};

#endif //KDE_SUB_QUADRATIC_KDE_H
