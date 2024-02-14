//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_H
#define KDE_SUB_QUADRATIC_KDE_H

#include "kernel.h"

//libraries
#include <vector>
#include <list>

//specific needs from libraries
using std::list;
using std::vector;


template<Arithmetic T>
class KDE {

public:
    virtual T QueryNewPoint(vector<T>) = 0;
};


#endif //KDE_SUB_QUADRATIC_KDE_H
