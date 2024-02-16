//
// Created by cj on 16-2-24.
//

#include "KDE.h"



using kernelFunction::Arithmetic;


#ifndef KDE_SUB_QUADRATIC_MULTI_LEVELKDE_H
#define KDE_SUB_QUADRATIC_MULTI_LEVELKDE_H

template <Arithmetic T>
class multi_levelKDE{
public:
    virtual void construct(list<vector<T>>, long double epsilon) = 0;
};
#endif //KDE_SUB_QUADRATIC_MULTI_LEVELKDE_H
