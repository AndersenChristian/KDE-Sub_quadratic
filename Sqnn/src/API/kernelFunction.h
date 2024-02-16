//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KERNELFUNCTION_H
#define KDE_SUB_QUADRATIC_KERNELFUNCTION_H

#include <functional>
#include <cmath>
#include <vector>
#include "ENUM/kernelType.h"

using std::vector;

//dealing with what equations to use
//
//Done since I expect that the library used will change later based on efficiency and data types
namespace mat{
    using std::sqrt;
    using std::pow;
    using std::abs;
    using std::exp;
}

using kernelType::Gaussian;
using kernelType::Exponential;
using kernelType::Laplacian;

namespace kernelFunction {

    // Concept to limit T to arithmetic types
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template<typename T>
    using kernelLambda = std::function<T(vector<T>, vector<T>, double)>;

    template<Arithmetic T>
    inline kernelLambda<T> kernel_function(kernelType kernel) {
        switch (kernel) {
            case Gaussian:
                return kernelLambda<T>([](vector<T> x, vector<T> y, double sigma) -> T {

                    T sum = 0;
                    for (int i = 0; i < x.size(); i++)
                        sum += mat::pow(mat::abs(x[i] - y[i]), 2);
                    sum = mat::sqrt(sum/(2* pow(sigma,2)));
                    return mat::exp(-sum);
                });
            case Exponential:
                break;
            case Laplacian:
                break;
        }

        //default (unreachable only there to avoid compiler warning)
        return [](vector<T> x, vector<T> y, double) -> T { return T{}; };
    }
}

#endif //KDE_SUB_QUADRATIC_KERNELFUNCTION_H
