//
// Created by cj on 10-4-24.
//

#ifndef KDE_SUB_QUADRATIC_CONTROLLER_H
#define KDE_SUB_QUADRATIC_CONTROLLER_H

#include <Eigen/Dense>
#include <iostream>

#include "kernelFunction.h"
#include "KdeUsingMrpt.h"
#include "KdeNaive.h"
#include "sample.h"
#include "KDE.h"

void buildMultiKDE(Eigen::MatrixXf data, std::vector<std::unique_ptr<KDE>> &kde, int index, int k,
									 int samples, int trees, kernel::kernelLambda<float> *kernel) {
	if (data.cols() <= samples) {
		//TODO: create something different and return
		return;
	}
	kde[index] = std::make_unique<KdeUsingMrpt>(data, k, samples, trees, kernel);
	buildMultiKDE(data.block(0, 0, data.rows(), std::ceil(data.cols()/2)), kde, index * 2, k, samples, trees, kernel);
	buildMultiKDE(data.block(0, data.cols() / 2, data.rows(), data.cols()/2), kde, index * 2 + 1, k, samples, trees, kernel);
}

void
runCppStyle(const Eigen::MatrixXf &data, const int vertices, [[maybe_unused]] const int dimensions,
						const int nearestNeighbor,
						const int samples, const int trees, [[maybe_unused]] const float rho, const double sigma,
						const double epsilon) {
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);

	//TODO: multi-level KDE instead
	//how tall is the tree?
	const int normalHeight = std::ceil(log2(vertices));
	const int cutoffHeight = std::ceil(log2(samples));
	const int treeHeight = normalHeight - cutoffHeight;
	const int nodes = (int) std::pow(2, treeHeight);
	std::vector<std::unique_ptr<KDE>> kdeTree(nodes);

	buildMultiKDE(data, kdeTree, 1, nearestNeighbor, samples, trees, &kernel);

	//testing how it went, expecting these to be fairly similar.
	printf("KDE full: %f\n", kdeTree[1]->query(data.col(1)));
	printf("KDE 1'st: %f\n", kdeTree[2]->query(data.col(1)));
	printf("KDE 2'nd: %f\n", kdeTree[3]->query(data.col(1)));
	printf("KDE sum : %f\n", (1. / 2.) * ( kdeTree[2]->query(data.col(1)) + kdeTree[3]->query(data.col(1))));
	//printf("KDE 4'th: %f\n", kdeTree[4]->query(data.col(1)));
	//printf("KDE 5'th: %f\n", kdeTree[5]->query(data.col(1)));
	//printf("KDE 6'th: %f\n", kdeTree[6]->query(data.col(1)));
	//printf("KDE 7'th: %f\n", kdeTree[7]->query(data.col(1)));
	float sum = 0;
	for(int i = 4; i < 8; ++i){
		sum += kdeTree[i]->query(data.col(1));
	}
	printf("KDE l'2: %f\n", sum/4);


	/*

	for (int i = 1; i < nodes; ++i) {
		int layer = (int) std::log2(i) + 1;
		int NodesOnCurrentLayer = (int) (std::pow(2, layer - 1));
		//Start and End index of the data needed for this layer
		int dataStartIndex = vertices * (int) ((double) (i - NodesOnCurrentLayer) / NodesOnCurrentLayer);
		int dataPoints = (int) std::ceil(vertices * (1. / NodesOnCurrentLayer));
		//creates a sub matrix by reference without doing copying
		Eigen::MatrixXf subData = data.block(0, dataStartIndex, dimensions, dataPoints);
		kdeTree[i] = (dataPoints <= samples) ?
								 std::unique_ptr<KDE>(std::make_unique<KdeNaive>(subData, &kernel))
																				 :
								 std::unique_ptr<KDE>(std::make_unique<KdeUsingMrpt>(subData, nearestNeighbor,
																																		 samples, trees, &kernel));

	}
	 */



	//testing tree setup
	//kdeTree[1] = std::unique_ptr<KDE>(std::make_unique<KdeUsingMrpt>(data, nearestNeighbor, samples, trees, &kernel));
	//kdeTree[2] = std::make_unique<KdeNaive>(data, &kernel);

	//FOR TESTING ONLY
	//KdeUsingMrpt kde(data, nearestNeighbor, samples, trees, &kernel);

	/*
	//TODO: weight
	std::vector<float> vertexWeight(vertices);
	const float ownContribution = (float) (1.0 - epsilon) * kernel(data.col(0), data.col(0));
	degreeWeight(kdeTree[1].get(), vertexWeight.data(), ownContribution);

	//TODO: Sample vertex
	const int vertexSamplingNr = 2000;
	std::vector<int> vertexSampled(vertexSamplingNr);
	vertexSampling(vertices, vertexWeight.data(), vertexSamplingNr, vertexSampled.data());
	 */

	//TODO: sample edges and assign values

}

#endif //KDE_SUB_QUADRATIC_CONTROLLER_H
