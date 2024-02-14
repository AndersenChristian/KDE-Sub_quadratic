//
// Created by cj on 14-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_EXACT_H
#define KDE_SUB_QUADRATIC_KDE_EXACT_H

#include "KDE.h"
#include "kernel.h"

#include <iostream>

using std::function;


template<Arithmetic T>
class KDE_exact : KDE<T>{
protected:
    list<vector<T>> data;
    kernelFunction<T> kernelFuntion;
public:
    KDE_exact(kernelFunction<T> kernelFuntion, list<vector<T>> data){
        this->kernelFuntion = kernelFuntion;
        this->data = data;
    }
};

#endif //KDE_SUB_QUADRATIC_KDE_EXACT_H
