//
// Created by cj on 6-2-24.
//

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
	std::uniform_real_distribution<float> distribution(-1, 1);
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
	const int n = 10000, d = 2000, k = 100, m = 1000, trees = 100;
	const float sigma = 100;

	Eigen::MatrixXf X = Eigen::MatrixXf(d, n);
	Eigen::MatrixXf q = Eigen::VectorXf(d);
	generateRandomMatrix(X, q, d, n);

	KdeUsingMrpt kde(X, n, d, m, k, trees, sigma);
	std::cout << "init finished\n";


	float app, exact;
	double start = omp_get_wtime();
	app = kde.query(q);
	double between = omp_get_wtime();
	exact = kde.query_exact(q);
	double end = omp_get_wtime();

	std::cout << "\napprox: " << app << "\texact: " << exact;
	std::cout << "\nT-app: " << between - start << "\tT-exact: " << end - between << std::endl;

	return 0;
}