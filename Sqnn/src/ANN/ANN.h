//
// Created by cj on 26-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ANN_H
#define KDE_SUB_QUADRATIC_ANN_H

#include "Mrpt.h"
#include "../API/kernelFunction.h"

template<typename T>
using node = vector<std::pair<vector<T>,vector<T>>*>;

template<typename T>
class Ann {
public:

	Ann(T *X, kernelType kernel, double bandWidth, int numberOfTrees) :
	trees(new node<T>(numberOfTrees)){

	}


	~Ann() = default;

private:
	T *X;
	node<T> trees;


};

#endif //KDE_SUB_QUADRATIC_ANN_H
