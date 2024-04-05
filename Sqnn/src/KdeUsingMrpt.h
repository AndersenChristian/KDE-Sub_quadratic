//
// Created by cj on 26-3-24.
//

#ifndef KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
#define KDE_SUB_QUADRATIC_KDEUSINGMRPT_H

#include <ANN/Mrpt.h>
#include <cmath>
#include <utility>
#include <vector>
#include <chrono>

#include "kernelFunction.h"

class KdeUsingMrpt {
public:
	//TODO implementation
	//Should only handle allocation. Make method for isValid after.
	//TODO: include and save lambda for distance
	KdeUsingMrpt(Eigen::MatrixXf &data, int n, int d, int k, int samples, int trees, float sigma,
							 kernel::kernelLambda<float> kernel)
			: data(data), n(n), d(d), samples(samples), KNN(k), sigma(sigma), mrpt(data), kernel(std::move(kernel)) {

		//Needed for the ANN to be setup.
		mrpt.grow_autotune(TARGET_RECALL, KNN, trees);

		//random number-generator setup
		std::random_device rd;
		generator.seed(rd());
	}

	float query_exact(const Eigen::VectorXf q) {
		float sum;
#pragma omp parallel for reduction(+:sum)
		for (int i = 0; i < n; ++i) {
			sum += kernel(data.col(i), q);
		}
		return sum / (float) n;
	}


	float query(const Eigen::VectorXf q) {
		std::vector<int> ann_list(n);
		std::vector<float> distances(n);

		//Get candidates
		int numberOfCandidates;
		mrpt.query(q, ann_list.data(), nullptr, &numberOfCandidates);

		//compute NN contribution
		float sum_a = 0;
		//TODO. should q be a part of the omp? and if so, shared to private (overhead vs cache miss)
#pragma omp parallel for reduction(+: sum_a)
		for (int i = 0; i < numberOfCandidates; ++i) {
			int index = ann_list[i];
			sum_a += kernel(data.col(index), q);
		}
		sum_a /= (float) numberOfCandidates;


		//compute sample contribution
		float sum_b;
		int index;
#pragma omp parallel for private(index) reduction(+: sum_b)
		for (int i = 0; i < samples; ++i) {
			do {
				index = randomIndex(0, n);
			} while (std::find(ann_list.begin(), ann_list.begin() + numberOfCandidates, index) !=
							 ann_list.begin() + numberOfCandidates);
			sum_b += kernel(data.col(index), q);
		}
		sum_b /= (float) samples;

		//compute total approx
		return (float) numberOfCandidates / (float) n * sum_a + (float) (n - numberOfCandidates) / (float) n * sum_b;

	}


private:
	const int KNN;
	const double TARGET_RECALL = 0.5;
	const Eigen::MatrixXf data;
	const kernel::kernelLambda<float> kernel;
	Mrpt mrpt;
	std::default_random_engine generator;

	const int n, d, samples;
	const float sigma;

	inline int randomIndex(const int min, const int max) {
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(this->generator);
	}
};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
