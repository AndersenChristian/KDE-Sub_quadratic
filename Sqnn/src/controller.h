//
// Created by cj on 10-4-24.
//

/*NOTE
 * remember to remove any [maybe_unused] only there right now to suppress compiler warnings.
 */

#ifndef KDE_SUB_QUADRATIC_CONTROLLER_H
#define KDE_SUB_QUADRATIC_CONTROLLER_H

#include <Eigen/Dense>
#include <iostream>

#include "kernel_function.h"
#include "KdeUsingMrpt.h"
#include "KdeNaive.h"
#include "sample.h"
#include "KDE.h"

void buildMultiKDE(Eigen::MatrixXf data, std::vector<std::unique_ptr<KDE>> &kde, int index, int k,
                   int samples, int trees, kernel::kernelLambda<float> *kernel) {
  if (data.cols() <= samples) return;
  kde[index] = std::make_unique<KdeUsingMrpt>(data, k, samples, trees, kernel);
  //recursively creates the tree
  printf("iteration: %d\n", index);
  //TODO change this into a loop instead of recursion
  buildMultiKDE(data.block(0, 0, data.rows(), std::ceil(data.cols() / 2)), kde, index * 2, k, samples, trees, kernel);
  buildMultiKDE(data.block(0, data.cols() / 2, data.rows(), data.cols() / 2), kde, index * 2 + 1, k, samples, trees,
                kernel);
}

void
runCppStyle(const Eigen::MatrixXf &data, const int vertices, [[maybe_unused]] const int dimensions,
            const int nearestNeighbor,
            const int samples, const int trees, [[maybe_unused]] const float rho, const double sigma,
            [[maybe_unused]] const double epsilon) {
  kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);

  //TODO: multi-level KDE instead
  //how tall is the tree?
  const int normalHeight = std::ceil(log2(vertices));
  const int cutoffHeight = std::ceil(log2(samples));
  const int treeHeight = normalHeight - cutoffHeight;
  const int nodes = (int) std::pow(2, treeHeight);
  std::vector<std::unique_ptr<KDE>> kdeTree(nodes);
  printf("normal height: %d\n", normalHeight);
  printf("cutoff height: %d\n", cutoffHeight);
  printf("tree height: %d\n", treeHeight);
  printf("nodes: %d\n\n", nodes);
  kdeTree[1] = std::make_unique<KdeUsingMrpt>(data, nearestNeighbor, samples, trees, &kernel);

  //printf("begin KDE-tree construction\n");
  //buildMultiKDE(data, kdeTree, 1, nearestNeighbor, samples, trees, &kernel);
  //printf("build KDE tree\n");

  //testing how it went, expecting these to be fairly similar.
  //printf("KDE full: %f\n", kdeTree[1]->query(data.col(1)));
  //printf("KDE 1'st: %f\n", kdeTree[2]->query(data.col(1)));
  //printf("KDE 2'nd: %f\n", kdeTree[3]->query(data.col(1)));
  //printf("KDE sum : %f\n", (1. / 2.) * (kdeTree[2]->query(data.col(1)) + kdeTree[3]->query(data.col(1))));
  //printf("KDE 4'th: %f\n", kdeTree[4]->query(data.col(1)));
  //printf("KDE 5'th: %f\n", kdeTree[5]->query(data.col(1)));
  //printf("KDE 6'th: %f\n", kdeTree[6]->query(data.col(1)));
  //printf("KDE 7'th: %f\n", kdeTree[7]->query(data.col(1)));
  //float sum = 0;
  //for (int i = 4; i < 8; ++i) {
  //	sum += kdeTree[i]->query(data.col(1));
  //}
  //printf("KDE l'2: %f\n", sum / 4);


  //test distanceSampling for leaf notes
  //int out = proportionalDistanceSampling(data.col(1),data.block(0,0,128,500),kernel);
  //printf("%d\n", out);

  //test tree setup
  //kdeTree[1] = std::unique_ptr<KDE>(std::make_unique<KdeUsingMrpt>(data, nearestNeighbor, samples, trees, &kernel));
  //kdeTree[2] = std::make_unique<KdeNaive>(data, &kernel);

  //TODO: weight
  std::vector<float> vertexWeight(vertices);
  //const float ownContribution = (float) (1.0 - epsilon) * kernel(data.col(0), data.col(0));
  sample::DegreeWeight(kdeTree[1].get(), vertexWeight.data(), ownContribution);
  printf("weights gathered\n");

  /*
  //TODO: Sample vertex
  const int vertexSamplingNr = 2000;
  std::vector<int> vertexSampled(vertexSamplingNr);
  vertexSampling(vertices, vertexWeight.data(), vertexSamplingNr, vertexSampled.data());
  printf("sampled vertex\n");

  //TODO: sample edges
  std::vector<std::pair<int,int>> edgeSampled(vertexSamplingNr);
  edgeSampling(kdeTree, vertexSampled, data, edgeSampled.data(), kernel);
  printf("sampled edges\n");
  */

  //TODO: assign values to sparse graph

}

#endif //KDE_SUB_QUADRATIC_CONTROLLER_H
