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

class KdeUsingMrpt : public KDE {
public:
	//TODO Should only handle allocation. Make method for isValid after.
	KdeUsingMrpt(const Eigen::MatrixXf &data, int n, int k, int samples, int trees,
							 kernel::kernelLambda<float> *kernel)
			: KNN(k), data(data), kernel(kernel), mrpt(data), n(n), samples(samples) {

		//random number-generator setup
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		generator.seed(seed);

		//if no k is needed (low accuracy)
		//TODO: make sekund constructor since mrpt is still setup.
		if (k == 0) return;

		//Needed for the ANN to be setup.
		mrpt.grow_autotune(TARGET_RECALL, KNN, trees);
	}

	float query(const Eigen::VectorXf &q) override {
		if (KNN == 0) return randomSampleAndSum(q);
		std::vector<int> ann_list(n);

		//Get candidates
		int numberOfCandidates;
		mrpt.query(q, ann_list.data(), &numberOfCandidates);

		//compute NN contribution
		float sum_a = 0;
#pragma omp parallel for reduction(+: sum_a) shared(ann_list, numberOfCandidates, q) default(none)
		for (int i = 0; i < numberOfCandidates; ++i) {
			int index = ann_list[i];
			sum_a += (*kernel)(data.col(index), q);
		}
		sum_a /= (float) numberOfCandidates;


		//compute sample contribution
		float sum_b = 0;
		int index;
#pragma omp parallel for private(index) reduction(+: sum_b) shared(numberOfCandidates, ann_list, q) default(none)
		for (int i = 0; i < samples; ++i) {
			do {
				index = randomIndex(n);
			} while (std::find(ann_list.begin(), ann_list.begin() + numberOfCandidates, index) !=
							 ann_list.begin() + numberOfCandidates);
			sum_b += (*kernel)(data.col(index), q);
		}
		sum_b /= (float) samples;

		//compute total approx
		return (float) numberOfCandidates / (float) n * sum_a + (float) (n - numberOfCandidates) / (float) n * sum_b;

	}

	const Eigen::MatrixXf& getData() override {
		return data;
	}

	std::vector<float> getExactDistance(Eigen::VectorXf &q){
		std::vector<float> list;
		for(int i = 0; i < n; ++i){
			list[i] = (*kernel)(data.col(i),q);
		}
		return list;
	}


private:
	const int KNN;
	const double TARGET_RECALL = 0.9;
	const Eigen::MatrixXf &data;
	const kernel::kernelLambda<float> *kernel;
	Mrpt mrpt;
	std::mt19937 generator;

	const int n, samples;

	inline int randomIndex(const int max) {
		std::uniform_int_distribution<int> distribution(0, max);
		return distribution(this->generator);
	}

	inline float randomSampleAndSum(const Eigen::VectorXf &q) {
		float sum = 0;
		for (int i = 0; i < samples; ++i) {
			sum += (*kernel)(data.col(randomIndex(n)), q);
		}
		return sum / (float) samples;
	}
};

#endif //KDE_SUB_QUADRATIC_KDEUSINGMRPT_H
