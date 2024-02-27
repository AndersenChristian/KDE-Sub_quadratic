//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "src/API/kernelFunction.h"
#include "ANN/ANN.h"
#include "ENUM/kernelType.h"
#include "KDE/ml-KDE_array.h"

#include <iostream>
#include <vector>
#include <algorithm>
//#include <flann/flann.hpp>
#include "../lib/Eigen/Dense"
#include "ANN/Mrpt.h"

using std::function;
using std::vector;
using std::string;

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

	int n = 10000, d = 200, k = 10;
	double target_recall = 0.9;
	Eigen::MatrixXf X = Eigen::MatrixXf::Random(d, n);
	Eigen::MatrixXf q = Eigen::VectorXf::Random(d);

	Eigen::VectorXi indices(k), indices_exact(k);

	Mrpt::exact_knn(q, X, k, indices_exact.data());
	std::cout << indices_exact.transpose() << std::endl;

	Mrpt mrpt(X);
	mrpt.grow_autotune(target_recall, k);

	mrpt.query(q, indices.data());
	std::cout << indices.transpose() << std::endl;

	return 0;
}