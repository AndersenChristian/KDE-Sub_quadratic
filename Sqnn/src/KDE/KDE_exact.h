//
// Created by cj on 14-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_EXACT_H
#define KDE_SUB_QUADRATIC_KDE_EXACT_H

#include "KDE.h"

#include <iostream>

template<Arithmetic T>
class KDE_exact : public KDE<T>{
//variables
private:
    list<vector<T>> data;
    kernelFunction<T> kernelFuntion;
    double bandwidth;

//function
public:
    KDE_exact(kernelFunction<T> kernelFuntion, list<vector<T>> data);
    T QueryNewPoint(vector<T>) override;
private:
    inline T computeBandwidth();

};

#endif //KDE_SUB_QUADRATIC_KDE_EXACT_H
