//
// Created by cj on 14-2-24.
//

#include "KDE_exact.h"

using std::vector;
using kernel::Arithmetic;

template<Arithmetic T>
KDE_exact<T>::KDE_exact(kernel::kernelFunction<T> kernelFunction, list<vector<T>> data) {
    this->kernelFunction = kernelFunction;
    this->data = data;
}

template<Arithmetic T>
T KDE_exact<T>::QueryNewPoint(vector<T> point) {
    T sum = 0;

    for(const vector<T>& d : this->data){
        sum += this->kernelFunction(point, d);
    }
    sum /= this->data.size();
    return sum;
}

template<Arithmetic T>
inline T KDE_exact<T>::computeBandwidth() {
    int dimensions = this->data.front().size();
    T* means = new T[dimensions];
    T* squared_diff_sum = new T[dimensions];
    T* std_dev = new T[dimensions];

    //initialise all array with data 0.0
    std::fill(means, means + dimensions, 0.0);
    std::fill(squared_diff_sum, squared_diff_sum + dimensions, 0.0);
    std::fill(std_dev, std_dev + dimensions, 0.0);

    //Computes the mean of each dimension
    //summing
    for(const auto& vec: this->data){
        for(int i = 0; i < dimensions; i++){
            means[i] += vec[i];
        }
    }

    //dividing by datasize
    for(int i = 0; i < dimensions; i++){
        means[i] /= data.size();
    }


    //compute the sqrt difference
    //difference between each value and mean
    for(const auto& vec: data){
        for(int i = 0; i < dimensions; i++){
            T diff = vec[i] - means[i];
            squared_diff_sum[i] += diff * diff;
        }
    }





}

template class KDE_exact<double>;