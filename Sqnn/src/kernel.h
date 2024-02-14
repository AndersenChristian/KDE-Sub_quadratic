//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KERNEL_H
#define KDE_SUB_QUADRATIC_KERNEL_H

#include <functional>
#include <cmath>
#include <vector>

using std::vector;

//dealing with what equations to use
//
//Done since i expect that the library used will change later based on efficiency and data types
namespace mat{
    using std::sqrt;
    using std::pow;
    using std::abs;
    using std::exp;
}

enum class kernels {
    Gaussian,
    Exponential,
    Laplacian
};

namespace kernel {

    // Concept to limit T to arithmetic types
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template<typename T>
    using kernelFunction = std::function<T(vector<T>,vector<T>)>;

    template<Arithmetic T>
    inline kernelFunction<T> kernel_function(kernels kernel) {
        switch (kernel) {
            case kernels::Gaussian:
                return kernelFunction<T>([](vector<T> x, vector<T> y) -> T {
                    T sum = 0;
                    for (int i = 0; i < x.size(); i++)
                        sum += mat::pow(mat::abs(x[i] - y[i]), 2);
                    sum = mat::sqrt(sum);
                    return mat::exp(-sum);

                });
            case kernels::Exponential:
                break;
            case kernels::Laplacian:
                break;
        }

        //default (unreachable only there to avoid compiler warning)
        return [](vector<T> x, vector<T> y) -> T { return T{}; };
    }
}

#endif //KDE_SUB_QUADRATIC_KERNEL_H
