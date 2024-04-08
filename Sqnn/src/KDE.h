//
// Created by cj on 8-4-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_H
#define KDE_SUB_QUADRATIC_KDE_H

#include <Eigen/Dense>

class KDE{
public:
	virtual void query_exact(Eigen::VectorXf &) = 0;
	virtual void query(Eigen::VectorXf &) = 0;
};
#endif //KDE_SUB_QUADRATIC_KDE_H
