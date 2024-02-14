//
// Created by cj on 14-2-24.
//

#include "KDE_exact.h"

template<Arithmetic T>
KDE_exact<T>::KDE_exact(kernelFunction<T> kernelFuntion, list<vector<T>> data) {
    this->kernelFuntion = kernelFuntion;
    this->data = data;
}

template<Arithmetic T>
T KDE_exact<T>::QueryNewPoint(vector<T> point) {
    T sum = 0;

    for(const vector<T>& d : this->data){
        sum += this->kernelFuntion(point, d);
    }
    sum /= this->data.size();
    return sum;
}

template class KDE_exact<double>;