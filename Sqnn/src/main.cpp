//
// Created by cj on 6-2-24.
//

#include "kernel_function.h"

#include <vector>
#include <random>
#include <omp.h>
#include <string>
#include <unistd.h>
#include <cstdio>
#include <csignal>
#include <cstdlib>

#include "IoOperation.h"
#include "controller.h"

using std::function;
using std::vector;
using std::string;

FILE *outputFile;

// Signal handler function
void signalHandler(int signum) {
	printf("Received signal: %d\n", signum);
	// Flush stdout and sync file system
	fflush(stdout);
	fsync(fileno(stdout));

	fclose(outputFile);

	exit(signum);
}

// Function to generate a filename based on current time
std::string generateFilename() {
	// Get current time
	std::time_t currentTime = std::time(nullptr);

	// Convert time to struct tm
	std::tm* localTime = std::localtime(&currentTime);

	// Format the filename as YYMMDD-HHMM
	char filename[20]; // Sufficient size for YYMMDD-HHMM
	std::strftime(filename, sizeof(filename), "%y%m%d-%H%M", localTime);

	return std::string(filename);
}


int main(int argc, char *argv[]) {
	// Register signal handler for SIGSEGV (segmentation fault)
	signal(SIGSEGV, signalHandler);
	signal(SIGABRT, signalHandler);

	// Generate filename
	std::string filename = "output_file/" + generateFilename();

	// Open the file using the generated filename
	outputFile = freopen(filename.c_str(), "w", stdout);

	if (outputFile == nullptr) {
		// Handle error
		perror("Error opening file");
		return 1;
	}

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
		std::string _filename = ("../Sqnn/data/" + std::string(argv[4]));
		if (!io::LoadData(_filename, n, d, rho, h, X)) return -1;
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

	// Close the file stream if you no longer need it
	fclose(outputFile);

	return 0;
}