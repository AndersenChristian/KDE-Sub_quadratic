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
    virtual KDE<T>** getTreeRoot() = 0;
    /**
     *
     *
     * @param current Current position in tree
     * @param child what child to visit
     * @return a pointeer to where the pointer to the data can be found.
     */
    virtual inline KDE<T>** getChild(KDE<T>** current, unsigned int child) = 0;
};
#endif //KDE_SUB_QUADRATIC_MULTI_LEVELKDE_H
