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

/**
 * Interface for what method the KDE must support.
 *
 * This interface expect, that all needed data is given in the constructor.
 *
 * @tparam T must support Arithmatic
 */
template<Arithmetic T>
class KDE {

public:
    /**
     * query the approximated value of the new point, based on how the KDE was initially constructed.
     *
     * @return an apoximated distance
     */
    virtual T QueryNewPoint(vector<T>) = 0;
    virtual ~KDE() = default;
};


#endif //KDE_SUB_QUADRATIC_KDE_H
