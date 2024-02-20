//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_H
#define KDE_SUB_QUADRATIC_KDE_H

#include "src/API/kernelFunction.h"

//libraries
#include <vector>
#include <list>

//specific needs from libraries
using std::list;
using std::vector;
using kernelFunction::Arithmetic;


template<Arithmetic T>
class KDE {

public:
    virtual T QueryNewPoint(vector<T>) = 0;
    virtual void deallocate() = 0;
    virtual ~KDE() = default;
};


#endif //KDE_SUB_QUADRATIC_KDE_H
