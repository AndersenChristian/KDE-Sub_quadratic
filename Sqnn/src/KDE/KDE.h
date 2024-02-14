//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KDE_H
#define KDE_SUB_QUADRATIC_KDE_H

#include <vector>
#include <list>

// Concept to limit T to arithmetic types
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

//import from libaries
using std::list;
using std::vector;

template<Arithmetic T>
class KDE{
public:
    T QueryNewPoint(vector<T>);

private:
};


#endif //KDE_SUB_QUADRATIC_KDE_H
