//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "ANN/ANN.h"

#include <iostream>
#include <vector>
#include <algorithm>
//#include "ANN/Mrpt.h"
#include <chrono>

using std::function;
using std::vector;
using std::string;

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

vector<float> dif(const Eigen::MatrixXf &mat, const Eigen::MatrixXf &q, int n, int d){
	vector<float> dif(d);
	for (int i = 0; i < mat.rows(); ++i) {
		dif[i] = abs(mat(i, n)-q(i));
	}
	return dif;
}

int main(int argc, char *argv[]) {
	int n = 10000, d = 10, k = 10;
	double target_recall = 0.5;
	Eigen::MatrixXf X = Eigen::MatrixXf::Random(d, n);
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
	vector<float> difs = dif(X,q,indices_exact(0),d);
	printVector(difs);

	//dif squared for each dimension
	for(float *ptr = difs.data(); ptr < difs.end().base(); ++ptr ){
		*ptr *= *ptr;
	}
	printVector(difs);

	//sum and root
	float sum = 0;
	for(const float f : difs){
		sum += f;
	}

	std::cout << "sum as float: " << sum << "\n";
	auto sumD = (double) sum;
	std::cout << "sum as double: " << sumD << "\n";
	sumD = sqrt(sumD);
	std::cout << "sqrt distance: " << sumD << "\n";



	return 0;
}