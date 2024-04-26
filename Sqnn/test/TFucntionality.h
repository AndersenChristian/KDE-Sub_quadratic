//
// Created by cj on 28-2-24.
//

#ifndef KDE_SUB_QUADRATIC_TFUCNTIONALITY_H
#define KDE_SUB_QUADRATIC_TFUCNTIONALITY_H

#include "src/Mrpt.h"

#include <iostream>
#include <Eigen/Dense>

void correctness() {
	//test data
	const int numRows = 2000; // Outer vector size
	const int numCols = 3;    // Inner vector size

	// Define the range for random number generation
	const float minVal = -1.0f;
	const float maxVal = 1.0f;

	// Define the range for the first 10 points (closer to {0, 0, 0})
	const float minCloseVal = -0.01f;
	const float maxCloseVal = 0.01f;

	// Seed for random number generation
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define distributions for different ranges
	std::uniform_real_distribution<float> disClose(minCloseVal, maxCloseVal);
	std::uniform_real_distribution<float> disFirstHalf(minVal, -0.01f);
	std::uniform_real_distribution<float> disSecondHalf(0.01f, maxVal);



	// Define the 2D vector
	std::vector<std::vector<float>> data;

	// Generate the data for the 2D vector
	for (int i = 0; i < numRows; ++i) {
		std::vector<float> innerVec(numCols);
		for (int j = 0; j < numCols; ++j) {
			// Fill the first 10 points within the first range
			if (i < 10) {
				innerVec[j] = disClose(gen);
			}
				// Fill the rest of the points within the second range
			else {
				if (i < numRows / 2) {
					innerVec[j] = disFirstHalf(gen);
				} else {
					innerVec[j] = disSecondHalf(gen);
				}
			}
		}
		data.push_back(innerVec);
	}

	std::vector<float> point = {0.0f, 0.0f, 0.0f};
	int k = 10;
	int out[k];
	float out_distances[k];


	// Print the generated data
	Mrpt::exact_knn(point.data(), data[0].data(), numCols, numRows, k, out, out_distances);

	// Print the indices of k nearest neighbors
	std::cout << "Indices of " << k << " nearest neighbors: ";
	for (int i = 0; i < k; ++i) {
		std::cout << out[i] << " ";
	}
	std::cout << std::endl;

	// Optionally print the distances to k nearest neighbors
	std::cout << "Distances to " << k << " nearest neighbors: ";
	for (int i = 0; i < k; ++i) {
		std::cout << out_distances[i] << " ";
	}
	std::cout << std::endl;


}

void test2(){
	const int n = 2;
	const int d = 1;
	const int k = 1;

	vector<vector<float>> data = {
			{0.3f},
			{0.2f}
	};
	vector<float> point = {0.0f};
	int out[k];
	float out_distances[k];

	Mrpt::exact_knn(point.data(), data[0].data(),d, n, k, out, out_distances);

	std::cout << out[0] << "\t" << out_distances[0] << "\t" << data[0][out[0]];

}

void test3(){
	const int n = 2000;
	const int d = 100;
	const int k = 20;

	auto X = Eigen::MatrixXf(d,n);
	auto y = Eigen::VectorXf(d);

	for(int i = 0 ; i < d; i++){
		y(i) = -1.0f;
		for(int j = 0 ; j < n ; j++){
			X(i, j) = j%d == 0 ? -0.8f : 0.0f;
		}
	}

	Eigen::VectorXi indices(k), indices_exact(k);
	float out_distances[k];

	Mrpt::exact_knn(y, X, k, indices_exact.data(), out_distances);
	std::cout << indices_exact.transpose() << "\n";
	for(auto v : out_distances){
		std::cout << v << "\t";
	}

	//approx
	Mrpt mrpt(X);
	double target_recall = 0.999;
	mrpt.grow_autotune(target_recall, k);
	mrpt.query(y, indices.data());

	std::cout << "\n" <<indices.transpose() << "\n";
	for(auto v : indices){
		std::cout<< v << "\t";
	}


}

#endif //KDE_SUB_QUADRATIC_TFUCNTIONALITY_H
