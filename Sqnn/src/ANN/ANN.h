//
// Created by cj on 26-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ANN_H
#define KDE_SUB_QUADRATIC_ANN_H

#include "Mrpt.h"
#include "../API/kernelFunction.h"

template<typename T>
struct Node{
	vector<T> *point1, *point2;
	Node *left, *right;
};

template<typename T>
class Ann {
public:

	Ann(vector<vector<T>> *X, kernelType kernel, double bandWidth, int numberOfTrees):
	trees(numberOfTrees){
		bool successfullyCreatedTree = false;
		for (int i = 0; i < numberOfTrees; i = successfullyCreatedTree ? i++ : i) {
			successfullyCreatedTree = false;
			makeTree(trees[i], 1);

		}
	}


	~Ann() = default;

private:
	int makeTree(Node<T> *node ) {

	}
	vector<Node<T>> *trees;


};

#endif //KDE_SUB_QUADRATIC_ANN_H
