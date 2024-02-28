//
// Created by cj on 26-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ANN_H
#define KDE_SUB_QUADRATIC_ANN_H

#include "Mrpt.h"

template<class T>
class Ann {
private:
	Mrpt *mrpt;
public:
	Ann(T *data, const int d, const int n, const int k, const float epsilon) {
		Eigen::MatrixXf dataM(d, n);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < d; j++){
				dataM(j,i) = data[i*d + j];
			}
		}
		std::cout << dataM.transpose();

		mrpt = new Mrpt(dataM);
		std::cout <<"\n done \n";
	}

};

#endif //KDE_SUB_QUADRATIC_ANN_H
