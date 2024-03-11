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
	int k;
public:
	Ann(T *data, const int d, const int n, const int k, const float epsilon, double target_recall) : k(k) {
		Eigen::MatrixXf dataM(d, n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < d; j++) {
				dataM(j, i) = data[i * d + j];
			}
		}
		mrpt = new Mrpt(dataM);
		mrpt->grow_autotune(target_recall, k);
	}

	void exact(const float *q, int *out, float *out_distance = nullptr) {
		mrpt->exact_knn(q, this->k, out, out_distance);
	}

	void aprox(const float *q, int *out, float *out_distance = nullptr) {
		mrpt->query(q, out, out_distance);
		//mrpt->query(q, out);
	}


	~Ann(){
		delete mrpt;
	};

};

#endif //KDE_SUB_QUADRATIC_ANN_H
