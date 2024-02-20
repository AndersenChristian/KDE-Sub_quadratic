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
    kernelLambda<T> kernelFunction;
    vector<vector<T>>* data;
    unsigned int start;
    unsigned int end;
    double bandwidth;

//function
public:
    KDE_exact(
        kernelLambda<T> kernelFunction,
        vector<vector<T>>& data,
        unsigned int start,
        unsigned int end,
        double bandwidth
    ): kernelFunction(kernelFunction), data(&data), start(start), end(end), bandwidth(bandwidth) {};

    T QueryNewPoint(vector<T> point) override {
        T sum = 0;

        for(unsigned int i = start; i < end; i++){
            sum += this->kernelFunction(point, (*data)[i], bandwidth);
        }
        sum /= end-start;
        return sum;
    };

    ~KDE_exact() override = default;
private:
};

//template class KDE_exact<double>;

#endif //KDE_SUB_QUADRATIC_KDE_EXACT_H
