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
	const int n = 10000, d = 2000, k = 100, m = 2000, trees = 10;
	const float sigma = 5;
	kernel::type kernelType = kernel::type::Gaussian;
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernelType);

	Eigen::MatrixXf X = Eigen::MatrixXf(d, n);
	Eigen::MatrixXf q = Eigen::VectorXf(d);
	generateRandomMatrix(X, q, d, n);

	double cStart = omp_get_wtime();
	KdeUsingMrpt kde(X, n, d, m, k, trees, sigma, kernel);
	double cEnd = omp_get_wtime();
	std::cout << "init finished\nIt took: " << cEnd - cStart << "\n";


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