//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KERNELFUNCTION_H
#define KDE_SUB_QUADRATIC_KERNELFUNCTION_H

#include <functional>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
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

    /**
     * Concept to limit T to arithmetic types
     * Might be set explicit to some datatypes later
     *
     * @tparam T Any type that support arithmatic operations
     */
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    /**
     * Declaring of the lambda function that is used as the kernel function for the KDE. \n
     * lambda follows the format of: Lambda((vector<T>, vector<T>, double) -> T) \n
     * takes 2 vectors of equal dimensions, and a bandwidth sigma and returns the distance. \n
     * \n
     * @tparam T support Arithmatic
     * @param x, y both need to be same size, else it may lead to unwanted behavior.
     * @param sigma bandwidth, must be greater then 0
     */
    template<Arithmetic T>
    using kernelLambda = std::function<T(Eigen::VectorXf x, Eigen::VectorXf y, double signma)>;

    /**
     * Given an ENUM of kernelType returns the corresponding lambda functions
     *
     * @tparam T of Arithmatic
     * @param kernel kernelType
     * @return kernelLambda of type T
     */
    template<Arithmetic T>
    inline kernelLambda<T> kernel_function(kernelType kernel) {
        switch (kernel) {
            //Gaussian kernel = e^(-||x-y||^2) where ||x-y||^2 is second norm
            //Second norm = sqrt(|x0-y0|^2 + ... + |xd-yd|^2)
            case Gaussian:
                return kernelLambda<T>([](Eigen::VectorXf x, Eigen::VectorXf y, double sigma) -> T {
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
        return [](Eigen::VectorXf x, Eigen::VectorXf y, double) -> T { return T{}; };
    }
}

#endif //KDE_SUB_QUADRATIC_KERNELFUNCTION_H
