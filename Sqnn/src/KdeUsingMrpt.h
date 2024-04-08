//
// Created by cj on 26-3-24.
//

#ifndef KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
#define KDE_SUB_QUADRATIC_KDEUSINGMRPT_H

#include <ANN/Mrpt.h>
#include <utility>
#include <vector>

#include "kernelFunction.h"
#include "KDE.h"

class KdeUsingMrpt : KDE{
public:
	//TODO Should only handle allocation. Make method for isValid after.
	KdeUsingMrpt(Eigen::MatrixXf &data, int n, int d, int k, int samples, int trees,
							 kernel::kernelLambda<float> kernel)
			: data(data), n(n), d(d), samples(samples), KNN(k), mrpt(data), kernel(std::move(kernel)) {

		//random number-generator setup
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		generator.seed(seed);

		//if no k is needed (low accuracy)
		//TODO: make sekund constructor since mrpt is still setup.
		if(k == 0) return;

		//Needed for the ANN to be setup.
		mrpt.grow_autotune(TARGET_RECALL, KNN, trees);
	}

	float query_exact(const Eigen::VectorXf &q) {
		float sum;
#pragma omp parallel for reduction(+:sum)
		for (int i = 0; i < n; ++i) {
			sum += kernel(data.col(i), q);
		}
		return sum / (float) n;
	}


	float query(const Eigen::VectorXf &q) {
		if(KNN == 0) return randomSampleAndSum(q);
		std::vector<int> ann_list(n);

		//Get candidates
		int numberOfCandidates;
		mrpt.query(q, ann_list.data(), nullptr, &numberOfCandidates);

		//compute NN contribution
		float sum_a = 0;
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
	const double TARGET_RECALL = 0.9;
	const Eigen::MatrixXf data;
	const kernel::kernelLambda<float> kernel;
	Mrpt mrpt;
	std::mt19937 generator;

	const int n, d, samples;

	inline int randomIndex(const int min, const int max) {
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(this->generator);
	}

	inline float randomSampleAndSum(const Eigen::VectorXf &q){
		float sum = 0;
		for(int i = 0; i < samples; ++i){
			sum += kernel(data.col(randomIndex(0,n)),q);
		}
		return sum / (float) samples;
	}
};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
