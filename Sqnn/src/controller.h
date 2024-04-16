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

void
runCppStyle(const Eigen::MatrixXf &data, const int vertices, [[maybe_unused]] const int dimensions,
						const int nearestNeighbor,
						const int samples, const int trees, [[maybe_unused]] const float rho, const double sigma,
						const double epsilon) {
	kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);

	//TODO: multi-level KDE instead
	//how tall is the tree?
	const int treeHeight = std::ceil(std::log2(vertices / samples));
	const int nodes = (int) std::pow(2, treeHeight + 1);
	std::vector<std::unique_ptr<KDE>> kdeTree;
	kdeTree.resize(nodes);


	/*
	for (int i = 1; i < nodes; ++i) {
		int layer = (int) std::log2(i);
		int NodesOnCurrentLayer = (int) (std::pow(2, layer));
		//Start and End index of the data needed for this layer
		int dataStartIndex = vertices * (int) ((double) (i - NodesOnCurrentLayer) / NodesOnCurrentLayer);
		int dataPoints = i % 2 == 0 ? (int) std::ceil(vertices * (1. / NodesOnCurrentLayer)) : vertices * (int) ((1. / NodesOnCurrentLayer));
		//creates a sub matrix by reference without doing copying
		Eigen::MatrixXf subData = data.block(0, dataStartIndex, dimensions, dataPoints);
		kdeTree[i] = (dataPoints <= samples) ?
								 std::make_unique<KdeNaive>(subData, &kernel)
																				 : //TODO: change to something else that works the way we needed.
								 std::unique_ptr<KDE>(std::make_unique<KdeUsingMrpt>(subData, nearestNeighbor,
																																		 samples, trees, &kernel));

	}
	 */

	kdeTree[1] = std::unique_ptr<KDE>(std::make_unique<KdeUsingMrpt>(data, nearestNeighbor,
																																	 samples, trees, &kernel));


	KdeUsingMrpt kde(data, nearestNeighbor, samples, trees, &kernel);

	printf("here\n");
	//TODO: weight
	std::vector<float> vertexWeight(vertices);
	printf("here\n");
	const float ownContribution = (float) (1.0 - epsilon) * kernel(data.row(0), data.row(0));
	printf("here\n");
	degreeWeight(kdeTree[1].get(), vertexWeight.data(), ownContribution);
	for(int i = 0; i < 5; ++i){
		printf("%f\n",vertexWeight[i]);
	}

	//TODO: Sample vertex
	const int vertexSamplingNr = 2000;
	std::vector<int> vertexSampled(vertexSamplingNr);
	vertexSampling(vertices, vertexWeight.data(), vertexSamplingNr, vertexSampled.data());

	//TODO: sample edges and assign values

}

#endif //KDE_SUB_QUADRATIC_CONTROLLER_H
