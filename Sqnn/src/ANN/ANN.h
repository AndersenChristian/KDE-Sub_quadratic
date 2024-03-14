//
// Created by cj on 26-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ANN_H
#define KDE_SUB_QUADRATIC_ANN_H

#include "Mrpt.h"
#include "../API/kernelFunction.h"

template<typename T>
using node = vector<std::pair<vector<T>, vector<T>> *>;

template<typename T>
class Ann {
public:

	Ann(T *X, kernelType kernel, double bandWidth, int numberOfTrees) :
			trees(new node<T>(numberOfTrees)) {
		for(auto *t: trees) t = new node<T>;
		bool successfullyCreatedTree = false;
		for (int i = 0; i < numberOfTrees; i = successfullyCreatedTree ? i++ : i) {
			successfullyCreatedTree = false;
			makeTree(trees[i], 1);

		}
	}


	~Ann() = default;

private:
	int makeTree(node<T> *tree, int currentIndex) {
		
	}

	T *X;
	node<T> trees;


};

#endif //KDE_SUB_QUADRATIC_ANN_H
