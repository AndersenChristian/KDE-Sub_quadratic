//
// Created by cj on 6-2-24.
//

#include "kernelFunction.h"

#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include <filesystem>
#include <fstream>
#include <string>


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

std::vector<std::string> splitString(const std::string& str) {
	std::istringstream iss(str);
	std::vector<std::string> tokens;
	std::string token;
	while (iss >> token) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}
	return tokens;
}

int main(int argc, char *argv[]) {
	int n, d;
	const int k = 1000, m = 2000, trees = 10;
	const float sigma = 2000;
	kernel::type kernelType = kernel::type::Gaussian;
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernelType);

	Eigen::MatrixXf X;
	Eigen::MatrixXf q;

	printf("%d\n\n", argc);

	//read data vs autogen
	if (argc > 1) {
		std::string filename = ("../Sqnn/data/" + std::string(argv[1]));
		// Open the file
		std::ifstream file(filename);

		// Check if the file opened successfully
		if (file.is_open()) {
			//TODO: loop and safe.
			//TODO: maybe parallel? not sure yet, or if that could cause mem issue
			std::string line;
			std::string token;
			std::stringstream ss(line);

			std::getline(file, line);
			std::vector<string> data = splitString(line);
			n = std::stoi(data[0]);
			d = std::stoi(data[1]);

			printf("%d\t%d", n, d);

			X = Eigen::MatrixXf(d,n);

			for(int i = 0; i < n; ++i){
				std::getline(file,line);
				data = splitString(line);
				for(int j = 0; j < data.size(); ++j)
					X(j,i) = std::stof(data[j]);
			}

		} else {
			printf("couldn't find file.\nshutdown...");
			return -1;
		}
		file.close();


		//return 0;
	} else {
		n = 10000;
		d = 1000;
		X = Eigen::MatrixXf(d, n);
		q = Eigen::VectorXf(d);
		generateRandomMatrix(X, q, d, n);
	}

	double cStart = omp_get_wtime();
	KdeUsingMrpt kde(X, n, d, m, k, trees, sigma, kernel);
	double cEnd = omp_get_wtime();


	float app, exact;
	double start = omp_get_wtime();
	app = kde.query(q);
	double between = omp_get_wtime();
	exact = kde.query_exact(q);
	double end = omp_get_wtime();

	//std::cout << "\napprox: " << app << "\texact: " << exact;
	//std::cout << "\nT-app: " << between - start << "\tT-exact: " << end - between << std::endl;

	//print section
	printf("program execution completed\n");
	printf("meta data:\n");
	printf("n: %d\td: %d\n", n, d);
	printf("(k)-nn: %d\ttrees: %d\n", k, trees);
	printf("samples: %d\t sigma: %f\n\n", m, sigma);

	printf("data analysis:\n");
	printf("expected KDE-value: %e\n", exact);
	printf("approx value: %e\n", app);
	printf("actual difference: %e\n", std::abs(app - exact));
	printf("precision: %e\n\n", std::abs(1 - (app / exact)));

	printf("Times\n");
	printf("nearest neighbor construction time: %.3e seconds\n", cEnd - cStart);
	printf("exact compute time: %.3e seconds\n", between - start);
	printf("approx compute time: %.3e seconds\n", end - between);
	printf("gained speedup: %.3e seconds\n", (between - start) - (end - between));
	printf("time taken for approx vs actual in percent decimal: %.3f\n", (end - between) / (between - start));


	return 0;
}