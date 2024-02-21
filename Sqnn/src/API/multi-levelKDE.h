//
// Created by cj on 16-2-24.
//

#include "KDE.h"

#include <utility>

using kernelFunction::Arithmetic;
using std::pair;


#ifndef KDE_SUB_QUADRATIC_MULTI_LEVELKDE_H
#define KDE_SUB_QUADRATIC_MULTI_LEVELKDE_H

template <Arithmetic T>
class multi_levelKDE{
public:
    virtual KDE<T>** getKDE() = 0;
};
#endif //KDE_SUB_QUADRATIC_MULTI_LEVELKDE_H
