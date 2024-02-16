//
// Created by cj on 14-2-24.
//

#include "KDE_exact.h"

using std::vector;
using kernel::Arithmetic;

template<Arithmetic T>
KDE_exact<T>::KDE_exact(kernel::kernelFunction<T> kernelFunction, list<vector<T>> data, double bandwidth) {
    this->kernelFunction = kernelFunction;
    this->data = data;
    this->bandwidth = bandwidth;
}

template<Arithmetic T>
T KDE_exact<T>::QueryNewPoint(vector<T> point) {
    T sum = 0;

    for(const vector<T>& d : this->data){
        sum += this->kernelFunction(point, d, bandwidth);
    }
    sum /= this->data.size();
    return sum;
}

template class KDE_exact<double>;