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

namespace Sample {
  using pair = std::pair<float, unsigned long>;


  struct Bucket {
    unsigned long index[2]{};
    float value[2]{};

    Bucket() = default;

    explicit Bucket(pair var) {
      index[0] = var.second;
      value[0] = var.first;
    }

    Bucket(pair var1, pair var2) {
      index[0] = var1.second;
      value[0] = var1.first;
      index[1] = var2.second;
      value[1] = var2.first;
    }
  };


  std::vector<float> DegreeWeight(const Eigen::MatrixXf &points, KDE &kde, const float ownContribution) {
    std::vector<float> out(points.cols());
    for (int i = 0; i < points.cols(); ++i)
      out[i] = kde.query(points.col(i)) - (ownContribution / (float) points.cols());
    return out;
  }

  //Note: might need overload method for single data
  inline Bucket SetupBucket(pair &min_pair, pair &max_pair, const float bucket_size) {
    Bucket bucket(min_pair, max_pair);
    float remaining_size = bucket_size - min_pair.first;
    bucket.value[1] = remaining_size;
    max_pair.first -= remaining_size;
    return bucket;
  }

  std::pair<float, unsigned long> HeapPop(std::multiset<std::pair<float, unsigned long>> &heap) {
    auto pair = *heap.begin();
    heap.erase(pair);
    return pair;
  }

  std::pair<float, unsigned long> HeapPop(std::multiset<std::pair<float, unsigned long>, std::greater<>> &heap) {
    auto pair = *heap.begin();
    heap.erase(pair);
    return pair;
  }

  std::vector<Bucket> CreateSampleBuckets(const std::vector<float> &degree_weight, const float degree_weight_sum) {
    float bucket_size = degree_weight_sum / (float) degree_weight.size();

    //setup and populate a min and a max heap
    std::multiset<pair> min_heap;
    std::multiset<pair, std::greater<>> max_heap;
    for (unsigned long i = 0; i < degree_weight.size(); ++i) {
      if (degree_weight[i] >= bucket_size)
        max_heap.insert(pair(degree_weight[i], i));
      else
        min_heap.insert(pair(degree_weight[i], i));
    }

    std::vector<Bucket> buckets(degree_weight.size());

    unsigned long i = 0;
    for (; i < buckets.size(); ++i) {
      if (min_heap.empty()) break;
      pair min = HeapPop(min_heap);
      pair max = HeapPop(max_heap);
      buckets[i] = SetupBucket(min, max, bucket_size);
      if (max.first == 0) continue;
      if (max.first >= bucket_size)
        max_heap.insert(max);
      else
        min_heap.insert(max);
    }

    for (; i < buckets.size(); ++i) {
      buckets[i] = Bucket(HeapPop(max_heap));
    }

    return buckets;
  }


  inline std::vector<unsigned long>
  VertexSampling(const std::vector<Bucket> &Buckets, const unsigned long samples, const float bucket_size) {
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned long> Dist_bucket(0, Buckets.size() - 1);
    std::uniform_real_distribution<float> Dist_internal_bucket(0, bucket_size);
    std::vector<unsigned long> Vertices_index(samples);

    for (auto &val: Vertices_index) {
      auto bucket_nr = Dist_bucket(rng);
      auto internal_bucket = Dist_internal_bucket(rng);
      val = Buckets[bucket_nr].value[0] > internal_bucket ?
            Buckets[bucket_nr].index[0] :
            Buckets[bucket_nr].index[1];
    }
    return Vertices_index;
  }

  inline void ShuffleMatrixRows(
      Eigen::MatrixXf &data,
      long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count()
  ){
    for(int i = 0; i < data.cols(); ++i){
      std::mt19937 rng(seed);
      std::uniform_int_distribution<long> getIndex(i, data.cols() - 1);
      long index = getIndex(rng);
      data.col(i).swap(data.col(index));
    }
  }

/*
  //DEBUG: still need to be looked at.

  //TODO: break into several smaller sections. very long atm.
  inline void
  edgeSampling(std::vector<std::unique_ptr<KDE>> &tree, std::vector<int> &vertices, const Eigen::MatrixXf &data,
               std::pair<int, int> *out, kernel::kernelLambda<float> &kernel) {
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
  */




}

#endif //KDE_SUB_QUADRATIC_SAMPLE_H
