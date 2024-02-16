//
// Created by cj on 14-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_EXACT_H
#define KDE_SUB_QUADRATIC_KDE_EXACT_H

#include "KDE.h"

#include <iostream>

template<kernel::Arithmetic T>
class KDE_exact : public KDE<T>{
//variables
private:
    list<vector<T>> data;
    kernel::kernelFunction<T> kernelFunction;
    double bandwidth;

//function
public:
    KDE_exact(
        kernel::kernelFunction<T>,
        list<vector<T>>,
        double
    );
    T QueryNewPoint(vector<T>) override;
private:
};

#endif //KDE_SUB_QUADRATIC_KDE_EXACT_H
