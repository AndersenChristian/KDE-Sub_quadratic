//
// Created by cj on 6-2-24.
//

#include "kernelFunction.h"

#include <vector>
#include <random>
#include <omp.h>
#include <string>

#include "IoOperation.h"
#include "controller.h"

using std::function;
using std::vector;
using std::string;


int main(int argc, char *argv[]) {
	int n, d;
	int k = 170, m = 500, trees = 10;
	double sigma = 5;
	float rho, h;
	kernel::type kernelType = kernel::type::Gaussian;
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernelType, sigma);
	Eigen::MatrixXf X;

	//limit amounts of threads and cores for omp
	omp_set_num_threads(1);

	//read data vs autogen
	if (argc == 5) {
		k = std::stoi(argv[1]);
		m = std::stoi(argv[2]);
		sigma = std::stod(argv[3]);
		std::string filename = ("../Sqnn/data/" + std::string(argv[4]));
		if (!io::loadData(filename, n, d, rho, h, X)) return -1;
	}

	runCppStyle(X, n, d, k, m, trees, rho, sigma, 0.01);

	return 0;
}