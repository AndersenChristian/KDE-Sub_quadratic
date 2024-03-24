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
#include <unordered_set>
#include <omp.h>

using std::function;
using std::vector;
using std::string;

std::default_random_engine generator;

template<typename T>
void printVector(vector<T> &data) {
	for (auto d: data) {
		std::cout << d << " ";
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
float computeTau(Eigen::MatrixXf &X, Eigen::MatrixXf &d, int n, double sigma) {
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

void generateRandomMatrix(Eigen::MatrixXf &X, Eigen::MatrixXf &q, int d, int n) {
	std::uniform_real_distribution<float> distribution(-2, 2);
	for (int i = 0; i < d; ++i) {
		for (int j = 0; j < n; ++j) {
			X(i, j) = distribution(generator);
		}
	}
	for (int i = 0; i < d; ++i) {
		q(i) = distribution(generator);
	}
}

bool hasValue(Eigen::VectorXi &k, int start, int end, int n) {
	if (start == end) return k[start] == n;
	int m = end - start / 2;
	if (k[m] < n) return hasValue(k, m + 1, end, n);
	return hasValue(k, start, m, n);
}

bool hasValue(const Eigen::VectorXi &candidates, int value) {
	int start = 0;
	int end = candidates.size() - 1;

	while (start <= end) {
		int mid = start + (end - start) / 2;
		if (candidates[mid] == value) {
			return true;
		} else if (candidates[mid] < value) {
			start = mid + 1;
		} else {
			end = mid - 1;
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	//basic variable
	//TODO get as arguments or from file.
	//TODO bigger n for testing, as the amount of sample required is to high for accurate result.
	const int n = 10000, d = 10, k = 1000, m = 1000;
	const double target_recall = 0.5;
	const float sigma = 5;


	seedEigenRandom();
	Eigen::MatrixXf X = Eigen::MatrixXf(d, n);
	Eigen::MatrixXf q = Eigen::VectorXf(d);
	generateRandomMatrix(X, q, d, n);

	Eigen::VectorXi indices(k);
	vector<float> distance(k);
	int numberOfCandidates;

	Mrpt mrpt(X);
	mrpt.grow_autotune(target_recall, k);

	mrpt.query(q, indices.data(), distance.data(), &numberOfCandidates);

	//output
	std::cout << "Indicies in order - size: " << indices.size() << "\n";
	std::cout << indices.transpose() << "\n\n";
	Eigen::VectorXi candidates(numberOfCandidates);
	for (int i = 0; i < numberOfCandidates; ++i) {
		candidates[i] = indices[i];
	}
	std::cout << "fixed candidate list - size: " << numberOfCandidates << "\n";
	std::cout << candidates.transpose() << "\n";

	//trying to get the actual KDE and approx KDE
	//TODO: stop using distance from ANN, and paralise into a sum instead (reduced mem)
	float sumA = 0, sumB;
	for (int i = 0; i < numberOfCandidates; ++i)
		sumA += exp(-distance[i] / sigma);


	vector<int> sampleIndexes(m);
	int sampleTaken = 0;
	std::sort(candidates.begin(), candidates.end());
	srand(time(nullptr));

	while (sampleTaken < m) {
		int sampleIndex = rand() % n; //TODO update to c++ 11 random generator.
		if (hasValue(candidates, sampleIndex))
			continue;
		sampleIndexes[sampleTaken++] = sampleIndex;
	}

	std::cout << "\n\nsample:\n";
	printVector<int>(sampleIndexes);
	auto kernel = kernelFunction::kernel_function<float>(kernelType::Gaussian);


	//sum of B
	float sum = 0;
	int index;
#pragma omp parallel for private(index) reduction(+: sum)
	for (int i = 0; i < m; ++i) {
		do {
			index = random() % n; //TODO: change to C11 random
		} while (std::find(sampleIndexes.begin(), sampleIndexes.end(), index) < sampleIndexes.end());
		sum += exp(-(X.col(sampleIndexes[i]) - q).squaredNorm() / sigma);
	}

	std::cout << "A: " << sumA << "\tB: " << sum;

	//actual KDE value vs aprox.
	sumA /= (float) k;
	sumB = (float) sum / m;
	float kdeAprox = (float) k / n * sumA + (float) (n - k) / n * sumB;
	std::cout << "\n\nKDEapprox: " << kdeAprox;

	//actual
	float sumKDE = 0;
#pragma omp parallel for reduction(+:sumKDE)
	for (int i = 0; i < n; ++i) {
		sumKDE += exp(-(X.col(i) - q).squaredNorm() / sigma);
	}
	sumKDE /= n;

	std::cout << "\tKDE: " << sumKDE;






	//tau calculation
	//float tau = computeTau<float>(X, n, 1);
	//std::cout << "\ntau:\t" << tau;

	//random sampling
	//int sampleAmount = 1 / ((epsilon * epsilon) * tau);
	//std::cout << "\n\nsample amount:\t" << sampleAmount;

	//sorting the index list of k nearest neighbor to reduce the kontrol time for all samples.
	//std::sort(indices.begin(), indices.end());


	/*
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
	//TODO something runs out of index around here.
	randomSampleDistanceSum /= sampleAmount;
	std::cout << "\n\nsampleSum=\t" << randomSampleDistanceSum;
	 */


	return 0;
}