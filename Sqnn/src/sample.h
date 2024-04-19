//
// Created by cj on 8-4-24.
//

#ifndef KDE_SUB_QUADRATIC_SAMPLE_H
#define KDE_SUB_QUADRATIC_SAMPLE_H

#include <vector>
#include <Eigen/Dense>
#include <chrono>
#include <random>
#include "kernelFunction.h"

#include "KDE.h"

inline void degreeWeight(KDE *kde, float *out, const float ownContribution) {
	const Eigen::MatrixXf &data = kde->getData();
#pragma omp parallel for shared(out, data, kde, ownContribution) default(none)
	for (int i = 0; i < data.cols(); ++i) {
		out[i] = kde->query(data.col(i)) - (1. / (float) data.cols()) * ownContribution;
	}
}

inline void vertexSampling(int n, float *in, int samples, int *out) {
	//turning the degree list into a running sum list such
	//p_i = sum(p_1..p_i)
	for (int i = 1; i < n; ++i) {
		in[i] += in[i - 1];
	}

	//TODO consider getting sample from other place, not sure as i think the size is redundent
	//random number-generator setup
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);

	int start, end, m;
	float a, b, v;
	for (int i = 0; i < samples; ++i) {
		start = 0, end = n - 1;
		while (start != end) {
			if (start < 0 || end < 0) std::exit(-2);
			m = start + ((end - start) / 2);
			a = start == 0 ? in[m] : in[m] - in[start - 1];  //ensures no out of index
			b = in[end] - in[m];
			std::uniform_real_distribution<float> distribution(0, a + b); //setup distribution limitation
			v = distribution(generator);
			if (v < a)
				end = m;
			else
				start = m + 1;
		}
		out[i] = start;
	}
}

inline int
proportionalDistanceSampling(const Eigen::VectorXf &q, const Eigen::MatrixXf &x, kernel::kernelLambda<float> &kernel) {
	int size = (int) x.cols();
	std::vector<float> weights(size);

	//creates a runnins sum
	weights[0] = kernel(q, x.col(0));
	for (int i = 1; i < size; ++i) {
		weights[i] = kernel(q, x.col(i)) + weights[i - 1];
	}

	//deal with random selection
	std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> distribution(0, weights[size - 1]);
	float ranValue = distribution(generator);

	int low = 0, high = size - 1, result = -1;

	while (low <= high) {
		int mid = low + (high - low) / 2;

		if (weights[mid] >= ranValue) {
			result = mid;
			high = mid - 1; // move to the left to find the smallest element
		} else {
			low = mid + 1;
		}
	}

	return result;
}

#endif //KDE_SUB_QUADRATIC_SAMPLE_H
