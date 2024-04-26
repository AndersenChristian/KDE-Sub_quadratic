//
// Created by cj on 8-4-24.
//

#ifndef KDE_SUB_QUADRATIC_SAMPLE_H
#define KDE_SUB_QUADRATIC_SAMPLE_H

#include <vector>
#include <Eigen/Dense>
#include <chrono>
#include <random>
#include "kernel_function.h"

#include "KDE.h"

namespace sample {

  //TODO: complete redo
  inline void DegreeWeight(KDE *kde, float *out, const float ownContribution) {
    const Eigen::MatrixXf &data = kde->getData();
#pragma omp parallel for shared(out, data, kde, ownContribution) default(none)
    for (int i = 0; i < data.cols(); ++i) {
      out[i] = kde->query(data.col(i)) - (1. / (float) data.cols()) * ownContribution;
    }
  }

  //NOTE: a bit long. might need to be split into several functions
  inline void vertexSampling(int n, float *in, int samples, int *out) {
    //turning the degree list into a running sum list such
    //p_i = sum(p_1..p_i)
    for (int i = 1; i < n; ++i) {
      in[i] += in[i - 1];
    }

    //TODO consider getting sample from other place, not sure as i think the size is redundent
    //random number-generator setup
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

    //TODO: more showing names. this make close to no sence.
    int start, end, m;
    float a, b, v;
    for (int i = 0; i < samples; ++i) {
      start = 0, end = n - 1;
      while (start != end) {
        if (start < 0 || end < 0) std::exit(-2);
        m = start + ((end - start) / 2);
        a = start == 0 ? in[m] : in[m] - in[start - 1];  //ensures no out of index
        b = in[end] - in[m];
        std::uniform_real_distribution<float> distribution(0, a + b); //setup distribution limitation
        v = distribution(generator);
        if (v < a)
          end = m;
        else
          start = m + 1;
      }
      out[i] = start;
    }
  }

  //NOTE: this might need to be rebuild to use O(n log n) in construction to be O(1) in running
  inline int
  proportionalDistanceSampling(const Eigen::VectorXf &q, const Eigen::MatrixXf &x,
                               kernel::kernelLambda<float> &kernel) {
    int size = (int) x.cols();
    std::vector<float> weights(size);

    //creates a runnins sum
    weights[0] = kernel(q, x.col(0));
    for (int i = 1; i < size; ++i) {
      weights[i] = kernel(q, x.col(i)) + weights[i - 1];
    }

    //deal with random selection
    std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> distribution(0, weights[size - 1]);
    float ranValue = distribution(generator);

    int low = 0, high = size - 1, result = -1;

    while (low <= high) {
      int mid = low + (high - low) / 2;

      if (weights[mid] >= ranValue) {
        result = mid;
        high = mid - 1; // move to the left to find the smallest element
      } else {
        low = mid + 1;
      }
    }

    return result;
  }

  //TODO: break into several smaller sections. very long atm.
  inline void
  edgeSampling(std::vector<std::unique_ptr<KDE>> &tree, std::vector<int> &vertices, const Eigen::MatrixXf &data,
               std::pair<int, int> *out,
               kernel::kernelLambda<float> &kernel) {
    //RNG generator for sampling
    std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> distribution;


    std::vector<std::pair<int, int>> active(vertices.size()); //<point index, tree index>
    std::vector<std::pair<int, int>> passive(vertices.size());//<point index, tree index>

    //index of the actual point working with
    int aCurrentPoint = 0;
    //index of the first empty position in the array or aCurrent depending on what is lowest.
    int aEmptyPoint = 0;
    //index of the position after the final data (potentially out of bounce of vector)
    int aFinishPoint = (int) vertices.size();
    //index of the last data in the array, -1 if empty
    int pTopPoint = -1;
    //index for where to put next digit in the out list
    int oPoint = 0;

    //fills the active list with all data, and index point to the root of the tree as a starting point.
    for (int i = 0; i < (int) vertices.size(); ++i) {
      active[i] = std::pair(vertices[i], 1);
    }

    //exits if both lists are empty.
    while (aFinishPoint != 0) {
      //debug prints
      printf("aCurrent: %d\n", aCurrentPoint);
      printf("aEmpty: %d\n", aEmptyPoint);
      printf("aFinish: %d\n", aFinishPoint);
      printf("pTop: %d\n", pTopPoint);
      printf("o: %d\n", oPoint);
      printf("sum: %d\n\n", pTopPoint + oPoint + aFinishPoint);
      //debug end

      //index of the current node position
      int currentNode = active[0].second;

      //checks if there exist any child notes, else this is leaf and has to be dealt with differently.
      if (currentNode * 2 < (int) tree.size()) {
        //reference to both children
        KDE &left = *tree[currentNode * 2];
        KDE &right = *tree[currentNode * 2 + 1];
        //runs for each entry in active.
        while (aCurrentPoint++ != aFinishPoint) {
          //grep the vector
          Eigen::VectorXf vertex = data.col(active[aCurrentPoint].first);
          //query its contribution to both childing
          float _a = left.query(vertex);
          float _b = right.query(vertex);

          //setup distribution for RNG
          distribution.param(std::uniform_real_distribution<float>::param_type(0, _a + _b));

          if (distribution(generator) < _a) {
            //going left, so it stays in active for next loop
            active[aCurrentPoint].second *= 2;
            active[aEmptyPoint++] = active[aCurrentPoint];
          } else {
            //going right, so it is pushed to passive until in becomes that nodes turn.
            passive[++pTopPoint] = std::pair(active[aCurrentPoint].first, currentNode * 2 + 1);
          }
        }
      } else {
        //it's a leaf
        //compute block size
        int rowStartNode = (int) std::pow(2, std::floor(std::log2(active[0].second)));
        int positionInRow = active[0].second % rowStartNode;
        int startCol = (int) ((double) data.cols() * ((double) positionInRow / (double) rowStartNode));
        int dataPoints = (int) ((double) data.cols() / (double) rowStartNode);

        //create block
        Eigen::MatrixXf block = data.block(0, startCol, data.rows(), dataPoints);

        //sample edges from the block
        while (aCurrentPoint != aFinishPoint) {
          int partner = proportionalDistanceSampling(data.col(active[aCurrentPoint].first), block, kernel);
          out[oPoint++] = std::pair(active[aCurrentPoint].first, partner);
          aCurrentPoint++;
        }

      }

      //todo move/delete this ugly shit
      if (pTopPoint == -1 && aEmptyPoint == 0) return;

      if (aEmptyPoint != 0) { //there is still elements in active
        aFinishPoint = aEmptyPoint;
      } else {
        //active list is empty, try and fill it
        int nextTree = passive[pTopPoint].second;
        aFinishPoint = 0;
        while (passive[pTopPoint].second == nextTree && pTopPoint != -1) {
          active[aFinishPoint++] = passive[pTopPoint--];
        }
      }
      //reset for next iteration
      aCurrentPoint = 0;
      aEmptyPoint = 0;
    }
  }

}

#endif //KDE_SUB_QUADRATIC_SAMPLE_H
