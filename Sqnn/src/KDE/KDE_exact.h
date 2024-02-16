//
// Created by cj on 14-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_EXACT_H
#define KDE_SUB_QUADRATIC_KDE_EXACT_H

#include "src/API/KDE.h"

#include <iostream>

using kernelFunction::Arithmetic;
using kernelFunction::kernelLambda;

template<Arithmetic T>
class KDE_exact : public KDE<T>{
//variables
private:
    list<vector<T>> data;
    kernelLambda<T> kernelFunction;
    double bandwidth;

//function
public:
    KDE_exact(
        kernelLambda<T> kernelFunction,
        list<vector<T>> data,
        double bandwidth
    ): kernelFunction(kernelFunction), data(data), bandwidth(bandwidth) {};

    T QueryNewPoint(vector<T> point) override {
        T sum = 0;

        for(const vector<T>& d : this->data){
            sum += this->kernelFunction(point, d, bandwidth);
        }
        sum /= this->data.size();
        return sum;
    };
private:
};

template class KDE_exact<double>;

#endif //KDE_SUB_QUADRATIC_KDE_EXACT_H
