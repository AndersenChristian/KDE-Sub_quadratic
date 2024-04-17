//
// Created by cj on 6-2-24.
//

#include "kernelFunction.h"

#include <vector>
#include <random>
#include <omp.h>
#include <string>
#include <unistd.h>

#include "IoOperation.h"
#include "controller.h"

using std::function;
using std::vector;
using std::string;


int main(int argc, char *argv[]) {

	int n, d;
	int k = 10, m = 500, trees = 10;
	double sigma = 5;
	float rho, h;
	kernel::type kernelType = kernel::type::Gaussian;
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernelType, sigma);
	Eigen::MatrixXf X;

	//limit amounts of threads and cores for omp
	omp_set_num_threads(8);

	//read data vs autogen
	if (argc == 5) {
		k = std::stoi(argv[1]);
		m = std::stoi(argv[2]);
		sigma = std::stod(argv[3]);
		std::string filename = ("../Sqnn/data/" + std::string(argv[4]));
		if (!io::loadData(filename, n, d, rho, h, X)) return -1;
	}

	runCppStyle(X, n, d, k, m, trees, rho, sigma, 0.01);

	/*
	//testing the Mrpt
	//int n = 10000, d = 200, k = 10;
	double target_recall = 0.9;
	//Eigen::MatrixXf X = Eigen::MatrixXf::Random(d, n);
	Eigen::MatrixXf q = X.col(1);

	Eigen::VectorXi indices(k), indices_exact(k);

	Mrpt::exact_knn(q, X, k, indices_exact.data());
	std::cout << indices_exact.transpose() << std::endl;

	Mrpt mrpt(X);
	mrpt.grow_autotune(target_recall, k);

	for(int i = 0; i < 1; ++i) {
		mrpt.query(q, indices.data(), nullptr);
		std::cout << indices.transpose() << std::endl;
	}
	 */


	/*
	// testing KDE
	KdeUsingMrpt kde(X, k, m, trees, &kernel);
	for (int i = 0; i < 1000; ++i) {
		printf("%d\t%f\n", i, kde.query(X.col(0)));
	}
	 */


	//sleep(1);





	/*TESTING Kernel (working)
	for(int i = 0; i < 100000; ++i){
		printf("%d\t%f\n",i,kernel(X.col(1), X.col(2)));
	}*/


	return 0;
}