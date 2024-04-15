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

inline std::vector<float> degreeWeight(KDE *kde, float *out, const float ownContribution) {
	const Eigen::MatrixXf &data = kde->getData();
	printf("dataSize: %ld", data.cols());
	std::vector<float> _out(data.cols());
	for (int i = 0; i < data.cols(); ++i) {
		_out[i] = kde->query(data.col(i)) - (1. / (float) data.rows()) * ownContribution;
	}
	return _out;
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
			m = (end - start) / 2;
			a = start == 0 ? in[m] : in[m] - in[start - 1];  //ensures no out of index
			b = in[end] - in[m];
			std::uniform_real_distribution<float> distribution(0, a + b); //setup distribution limitation
			v = distribution(generator);
			if (v < a)
				end -= m;
			else
				start += m + 1;
		}
		out[i] = start;
	}
}

#endif //KDE_SUB_QUADRATIC_SAMPLE_H
