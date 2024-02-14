//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_H
#define KDE_SUB_QUADRATIC_KDE_H

//libraries
#include <vector>
#include <list>

//specific needs from libraries
using std::list;
using std::vector;


// Concept to limit T to arithmetic types
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;


template<Arithmetic T>
class KDE {

public:
    T QueryNewPoint(vector<T>);

private:
};


#endif //KDE_SUB_QUADRATIC_KDE_H
