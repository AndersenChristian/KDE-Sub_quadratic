//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "src/API/kernelFunction.h"
#include "ANN/ANN.h"
#include "ENUM/kernelType.h"
#include "KDE/ml-KDE_array.h"
#include "../test/TFucntionality.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <Eigen/Dense>
//#include "ANN/Mrpt.h"
#include <chrono>

using std::function;
using std::vector;
using std::string;

std::tuple<int, int, int> test(int n, int d, int k);

int countSameValues(int arr1[], int arr2[], int size) {
	int count = 0;

	int j = 0; // Pointer for array A
	int k = 0; // Pointer for array B

	while (j < size && k < size) {
		if (arr1[j] == arr2[k]) {
			// If elements are equal, increment count and move both pointers
			++count;
			++j;
			++k;
		} else if (arr1[j] < arr2[k]) {
			// If element in A is smaller, move pointer for A
			++j;
		} else {
			// If element in B is smaller, move pointer for B
			++k;
		}
	}

	return count;
}

std::vector<float>* generateRandomData(int d, int n, std::mt19937 gen) {
	std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	// Create a vector of size d*n and fill it with random floats
	auto* data = new std::vector<float>(d * n);
	for (int i = 0; i < d * n; ++i) {
		(*data)[i] = dis(gen);
	}

	return data;
}

std::string intArrayToString(const int arr[], size_t size) {
	std::stringstream ss;
	for (size_t i = 0; i < size; ++i) {
		ss << arr[i] << " ";
	}
	return ss.str();
}

void manyTest() {
	const int k_start = 10, k_max = 51;
	int hits;
	const int tests = 500;
	int result[k_max - k_start];

	for (int k = k_start; k < k_max; k++) {
		hits = 0;
		int indexExact[k];
		int indexAprox[k];
		for (int i = 0; i < tests; i++) {
			//std::cout << "run: " << i << "\n";

			// Initialize random number generator
			std::random_device rd;
			std::mt19937 gen(rd());

			// Generate random values for d and n
			std::uniform_int_distribution<int> d_distribution(10, 30);
			std::uniform_int_distribution<int> n_distribution(500, 1000);
			int d = d_distribution(gen);
			int n = n_distribution(gen);

			//std::cout << "d: " << d << "\tn:" << n << "\n";

			vector<float> *data = generateRandomData(d, n, gen);
			//std::cout << "data gen success." << "\n";

			auto *ann = new Ann(data->data(), d, n, k, 0.5, 0.9);
			//std::cout << "Ann gen success." << "\n";

			auto *point = new vector<float>(d, 0.0f);
			//std::cout << "point gen success." << "\n";

			ann->exact(point->data(), indexExact);
			//std::cout << "exact data gen success." << "\n";

			ann->aprox(point->data(), indexAprox);
			//std::cout << "aprox data gen success." << "\n";

			std::sort(indexExact, indexExact + k);
			std::sort(indexAprox, indexAprox + k);

			int count = countSameValues(indexExact, indexAprox, k);

			//std::cout << "exact: " << intArrayToString(indexExact, k) << "\n";
			//std::cout << "aprox: " << intArrayToString(indexAprox, k) << "\n";
			//std::cout << count << "/" << k << "\n";

			hits += count;

			delete ann;
			delete data;
			delete point;
		}
		std::cout << "\nresult: " << hits << "\tmax: " << tests * k << "\n\n";
		result[k - k_start] = hits;
	}

	for (int i = 0; i < k_max - k_start; i++) {
		std::cout << "k: " << i + k_start << " = ";
		std::cout << ((float) result[i] / (float) (i + k_start)) / (float) tests << "\t";

	}
}

int main(int argc, char *argv[]) {

	/*
	vector<vector<double>> vectors = {
					{4.0},
					{4.0},
					{4.0},
					{4.0},
					{2.0},
					{2.0},
					{2.0},
					{2.0}
	};

	long double epsilon = 1;
	double sigma = 2;
	auto mlKDE = new ml_KDE_array<double>(&vectors, epsilon, sigma, Gaussian);

	auto kdeElement = mlKDE->getTreeRoot();
	std::cout << (*kdeElement)->QueryNewPoint({2.0}) << "\n";

	std::cout << ((kdeElement == (mlKDE->getChild(kdeElement,0))) ? 1 : 0 ) << "\n";

	kdeElement = mlKDE->getChild(kdeElement,0);

	std::cout << (*kdeElement)->QueryNewPoint({2.0}) << "\n";
	std::cout << (*(kdeElement + 1))->QueryNewPoint({2.0}) << "\n";

	kdeElement = mlKDE->getChild(kdeElement,0);

	std::cout << ((kdeElement == (mlKDE->getChild(kdeElement,0))) ? 1 : 0 ) << "\n";

	//std::cout << kde.second->QueryNewPoint({3.0});

	//std::cout << "KDE:" << sizeof(KDE<double>) << "\tKDE_EXACT:" << sizeof(KDE_exact<double>);



	//tear down \
	delete kde;

	// Clear all vectors and deallocate memory
	std::for_each(vectors.begin(), vectors.end(), [](std::vector<double>& inner_vec) {
			inner_vec.clear();
	});

	// Clear the outer vector
	vectors.clear();

	 */

	/*

	vector<vector<float>> vectors = {
					{4.0},
					{4.0},
					{4.0},
					{4.0},
					{2.0},
					{2.0},
					{2.0},
					{2.0}
	};

	auto *ann = new Ann<float>(vectors, 1);


	delete ann;

	*/

	/*
	int result[20](0);
	int timeAverage = 0;

	for(int i = 0; i < 100000; i++){
		auto data = test(100000, 200, 20);
		result[std::get<0>(data)] ++ ;
		//if(i == 0) timeAverage =
	}

 */

	//correctness();
	//test3();

	return 0;
}

/*
std::tuple<int, int, int> test(int n, int d, int k) {
	// Create a random number engine
	std::random_device rd;
	std::mt19937 gen(rd()); // Mersenne Twister engine

	// Create a uniform real distribution in the range [-1, 1]
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);


	//int n = 100000, d = 200, k = 20;

	Eigen::MatrixXf X = Eigen::MatrixXf::Random(d, n);
	Eigen::MatrixXf q = Eigen::VectorXf::Random(d);

	for (int i = 0; i < d; i++) {
		for (int j = 0; j < n; j++) {
			X(i, j) = dist(gen);
		}
	}

	for (int i = 0; i < d; i++) {
		q(i) = dist(gen);
	}

	//std::cout << test << "\n";

	double target_recall = 0.9;
	//Eigen::MatrixXf X = Eigen::MatrixXf::Random(d, n);
	//Eigen::MatrixXf q = Eigen::VectorXf::Random(d);

	//std::cout << X << "\n" ;

	Eigen::VectorXi indices(k), indices_exact(k);

	auto start = std::chrono::high_resolution_clock::now();
	Mrpt::exact_knn(q, X, k, indices_exact.data());
	auto end = std::chrono::high_resolution_clock::now();
	//std::cout << indices_exact.transpose() << "\t time:" << end-start << std::endl;

	Mrpt mrpt(X);
	mrpt.grow_autotune(target_recall, k);

	start = std::chrono::high_resolution_clock::now();
	mrpt.query(q, indices.data());
	end = std::chrono::high_resolution_clock::now();
	//std::cout << indices.transpose() << "\t time:" << end-start << std::endl;

	//Result:

	int count = 0;


	// Compare each element of vec1 with elements of vec2
	for (int i = 0; i < indices_exact.size(); ++i) {
		for (int j = 0; j < indices.size(); ++j) {
			if (indices_exact(i) == indices(j)) {
				count++;
				break; // Move to the next element of vec1
			}
		}
	}

	std::cout << count << "/" << k;

	return new std::tuple<float,float,float>(0.1,0.1,0.1);
}*/