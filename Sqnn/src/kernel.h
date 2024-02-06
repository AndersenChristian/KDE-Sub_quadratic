//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KERNEL_H
#define KDE_SUB_QUADRATIC_KERNEL_H

#include <functional>
#include <cmath>
#include <vector>
#include <iostream>

enum class kernel{
    Gaussian,
    Exponential,
    Laplacian
};

using std::vector;

//dealing with what equations to use
//
//Done since i expect that the library used will change later based on efficiency and data types
namespace mat{
    using std::sqrt;
}

using namespace mat;

// Concept to limit T to arithmetic types
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;


template<Arithmetic T>
std::function<T(vector<T>,vector<T>)> kernel_function(kernel kernel){
    switch (kernel) {
        case kernel::Gaussian:
            return [](vector<T> x, vector<T> y) -> T {
                T sum = 0;
                for(int i = 0; i < x.size(); i++)
                    sum += pow(abs(x[i]-y[i]),2);
                std::cout << sum << "\n";
                return sqrt(sum);
            };
        case kernel::Exponential:
            break;
        case kernel::Laplacian:
            break;
    }
}

#endif //KDE_SUB_QUADRATIC_KERNEL_H
