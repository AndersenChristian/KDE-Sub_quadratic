//
// Created by cj on 16-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ML_KDE_ARRAY_H
#define KDE_SUB_QUADRATIC_ML_KDE_ARRAY_H

//headers
#include "src/API/multi-levelKDE.h"
#include "KDE_exact.h"

//Libaries

//method
using kernelFunction::kernelLambda;


template<Arithmetic T>
class ml_KDE_array : multi_levelKDE<T>{
private:
    const long double epsilon; //maybe not const??
    const kernelLambda<T> kernel_function;
    const vector<KDE<T>*> kde;

public:
    ml_KDE_array
    (
        const vector<vector<T>>* data,
        const long double epsilon,
        double sigma, //might need to store sigma
        const kernelType kernelType
    ):
    epsilon(epsilon),
    kernel_function(kernelFunction::kernel_function<T>(kernelType::Gaussian)),
    kde(vector<KDE<T>>((int) pow(2, log(data->size())+1)))
    {
        //int size = pow(2, log(data->size())+1);
        //kde.resize(size);
        rec_construct(data, 0, data->size() - 1, sigma, 1);
    }

    ~ml_KDE_array() {
        delete[] kde;
    }

private:
    void rec_construct
    (
        const vector<vector<T>>* data,
        const unsigned int startPoint,
        const unsigned int endPoint,
        const double sigma,
        const unsigned int arrayIndex
    ){
        //check if done.
        if (endPoint - startPoint < 2) return;

        //creates this layers KDE
        kde[arrayIndex] = KDE_exact<T>(kernel_function, *data, startPoint, endPoint, 2);

        //computes the middle
        const unsigned int m = (startPoint + endPoint) / 2;
        //recursive left child
        rec_construct(data,startPoint,m,sigma,arrayIndex*2);
        //recursive right child
        rec_construct(data,m+1,endPoint,sigma,arrayIndex*2+1);
    }
};

#endif //KDE_SUB_QUADRATIC_ML_KDE_ARRAY_H
