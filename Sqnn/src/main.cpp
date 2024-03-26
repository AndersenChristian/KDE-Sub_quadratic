//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "ANN/ANN.h"
#include "kernelFunction.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_set>
#include <omp.h>

#include "KdeUsingMrpt.h"

using std::function;
using std::vector;
using std::string;

void generateRandomMatrix(Eigen::MatrixXf &X, Eigen::MatrixXf &q, int d, int n) {
	std::uniform_real_distribution<float> distribution(-2, 2);
	std::default_random_engine generator;
	std::random_device rd;
	generator.seed(rd());
	for (int i = 0; i < d; ++i) {
		for (int j = 0; j < n; ++j) {
			X(i, j) = distribution(generator);
		}
	}
	for (int i = 0; i < d; ++i) {
		q(i) = distribution(generator);
	}
}

int main(int argc, char *argv[]) {
	//basic variable
	//TODO get as arguments or from file.
	//TODO bigger n for testing, as the amount of sample required is to high for accurate result.
	const int n = 100000, d = 10, k = 1000, m = 5000;
	const float sigma = 5;

	Eigen::MatrixXf X = Eigen::MatrixXf(d, n);
	Eigen::MatrixXf q = Eigen::VectorXf(d);
	generateRandomMatrix(X, q, d, n);

	KdeUsingMrpt kde(X, n, d, m, k, sigma);

	std::cout << "a";

	float app, exact;
	app = kde.query(q);
	exact = kde.query_exact(q);

	std::cout << "approx: " << app << "\texact: " << exact << std::endl;

	return 0;
}