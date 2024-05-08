//
// Created by cj on 6-2-24.
//

#ifndef KDE_SUB_QUADRATIC_KERNEL_FUNCTION_H
#define KDE_SUB_QUADRATIC_KERNEL_FUNCTION_H

#include <functional>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

using std::vector;

//dealing with what equations to use
//
//Done since I expect that the library used will change later based on efficiency and data types
namespace mat {
	using std::sqrt;
	using std::pow;
	using std::abs;
	using std::exp;
}

namespace kernel {
	enum class type {
		Gaussian,
		Exponential,
		Laplacian
	};

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
	 */
	template<Arithmetic T>
	using kernelLambda = std::function<T(T distance)>;

	/**
	 * Given an ENUM of kernelType returns the corresponding lambda functions
	 *
	 * @tparam T of Arithmatic
	 * @param kernel kernelType
	 * @return kernelLambda of type T
	 */
	template<Arithmetic T>
	inline kernelLambda<T> kernel_function(kernel::type kernel, const double sigma) {
		switch (kernel) {
			//Gaussian kernel = e^(-||x-y||^2) where ||x-y||^2 is second norm
			//Second norm = sqrt(|x0-y0|^2 + ... + |xd-yd|^2)
			case kernel::type::Gaussian:
				return kernelLambda<T>([sigma](T distance) -> T {
					return mat::exp(-distance / sigma);
				});
			case kernel::type::Exponential:
        return kernelLambda<T>([sigma](T distance) -> T {
          T sum = std::sqrt(distance / sigma);
          return mat::exp(-sum);
        });
			case kernel::type::Laplacian:
				break;
		}

		//default (unreachable, only there to avoid compiler warning)
		return [](T) -> T { return T{}; };
	}

  inline Eigen::VectorXf KernelFunction(const kernel::type kernel, const float sigma, const Eigen::VectorXf &distances){
    switch (kernel) {
      case type::Gaussian:
        return distances.unaryExpr([sigma](float v){return std::exp(-v/sigma);});
      case type::Exponential:
        break;
      case type::Laplacian:
        break;
    }
  }
}

#endif //KDE_SUB_QUADRATIC_KERNEL_FUNCTION_H
