//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "ANN/ANN.h"
#include "API/kernelFunction.h"

#include <iostream>
#include <vector>
#include <algorithm>
//#include "ANN/Mrpt.h"
#include <chrono>
#include <random>

using std::function;
using std::vector;
using std::string;

std::default_random_engine generator;

void printVector(vector<float> &data) {
	for (auto d: data) {
		std::cout << d << "\t";
	}
	std::cout << "\n";
}

void printComparator(const Eigen::MatrixXf &mat, const Eigen::MatrixXf &q, int n) {
	for (int i = 0; i < mat.rows(); ++i) {
		std::cout << mat(i, n) << "\t" << q(i) << "\n";
	}
	std::cout << "\n\n";
}

vector<float> dif(const Eigen::MatrixXf &mat, const Eigen::MatrixXf &q, int n, int d) {
	vector<float> dif(d);
	for (int i = 0; i < mat.rows(); ++i) {
		dif[i] = abs(mat(i, n) - q(i));
	}
	return dif;
}

template<Arithmetic T>
float computeTau(Eigen::MatrixXf X, int n, double sigma) {
	auto kernel = kernelFunction::kernel_function<T>(kernelType::Gaussian);
	Eigen::VectorXf point1, point2;
	float tau = 1, distance;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i == j) continue;
			point1 = X.col(i), point2 = X.col(j);
			distance = kernel(point1, point2, sigma);
			tau = distance < tau ? distance : tau;
		}
	}
	return tau;
}

void seedEigenRandom() {
	// Use a random device to seed the generator
	std::random_device rd;
	generator.seed(rd());
}

Eigen::MatrixXf generateRandomMatrix(int d, int n) {
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	Eigen::MatrixXf randomMatrix(d, n);

	for (int i = 0; i < d; ++i) {
		for (int j = 0; j < n; ++j) {
			randomMatrix(i, j) = distribution(generator);
		}
	}

	return randomMatrix;
}

bool hasValue(Eigen::VectorXi& k, int start, int end, int n) {
	if(start == end) return k[n] == n;
	int m = end-start;
	if (k[m] < n) return hasValue(k, start, m-1, n);
	return hasValue(k,m,end,n);
}

int main(int argc, char *argv[]) {
	int n = 10000, d = 10, k = 10;
	double target_recall = 0.5;
	float epsilon = 0.01;
	seedEigenRandom();
	Eigen::MatrixXf X = generateRandomMatrix(d, n);
	Eigen::MatrixXf q = Eigen::VectorXf::Random(d);

	Eigen::VectorXi indices(k), indices_exact(k);
	vector<float> distance(k), distance_exact(k);

	Mrpt::exact_knn(q, X, k, indices_exact.data(), distance_exact.data());

	Mrpt mrpt(X);
	mrpt.grow_autotune(target_recall, k);

	mrpt.query(q, indices.data(), distance.data());

	//output
	std::cout << "Indicies in expected size:\n";
	std::cout << indices_exact.transpose() << "\n";
	std::cout << indices.transpose() << "\n\n";

	std::cout << "Actual distances:\n";
	printVector(distance_exact);
	printVector(distance);
	std::cout << "\n";

	//What are the distances?
	std::cout << "distance understanding:\n";
	printComparator(X, q, 0);

	std::cout << "difference in each dimension:\n";
	vector<float> difs = dif(X, q, indices_exact(0), d);
	printVector(difs);

	//dif squared for each dimension
	for (float *ptr = difs.data(); ptr < difs.end().base(); ++ptr) {
		*ptr *= *ptr;
	}
	printVector(difs);

	//sum and root
	float sum = 0;
	for (const float f: difs) {
		sum += f;
	}

	std::cout << "sum as float: " << sum << "\n";
	auto sumD = (double) sum;
	std::cout << "sum as double: " << sumD << "\n";
	sumD = sqrt(sumD);
	std::cout << "sqrt distance: " << sumD << "\n";

	//tau calculation
	float tau = computeTau<float>(X, n, 1);
	std::cout << "\ntau:\t" << tau;

	//random sampling
	int sampleAmount = 1 / ((epsilon * epsilon) * tau);
	std::cout << "\n\nsample amount:\t" << sampleAmount;

	//sorting the index list of k nearest neighbor to reduce the kontrol time for all samples.
	std::sort(indices.begin(), indices.end());


	float randomSampleDistanceSum = 0;
	std::uniform_int_distribution<int> distribution(0, n);
	int selectIndex;
	auto kernel = kernelFunction::kernel_function<float>(kernelType::Gaussian);
	for (int i = 0; i < sampleAmount; ++i) {
		do {
			selectIndex = distribution(generator);
			std::cout << selectIndex;
			if (hasValue(indices, 0, k, selectIndex)) continue;
			randomSampleDistanceSum += kernel(X.col(selectIndex), q, 1);
			break;
		} while (true);
	}
	randomSampleDistanceSum /= sampleAmount;
	std::cout << "\n\nsampleSum=\t" << randomSampleDistanceSum;


	return 0;
}