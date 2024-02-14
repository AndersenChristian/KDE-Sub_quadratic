//
// Created by cj on 14-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_EXACT_H
#define KDE_SUB_QUADRATIC_KDE_EXACT_H

#include "KDE.h"

#include <iostream>

template<Arithmetic T>
class KDE_exact : public KDE<T>{
private:
    list<vector<T>> data;
    kernelFunction<T> kernelFuntion;
public:
    KDE_exact(kernelFunction<T> kernelFuntion, list<vector<T>> data);
    T QueryNewPoint(vector<T>) override;



};

#endif //KDE_SUB_QUADRATIC_KDE_EXACT_H
