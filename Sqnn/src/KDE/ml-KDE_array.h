//
// Created by cj on 16-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ML_KDE_ARRAY_H
#define KDE_SUB_QUADRATIC_ML_KDE_ARRAY_H

//headers needed
#include "src/API/multi-levelKDE.h"
#include "KDE_exact.h"

//Libaries

//import from headers/libaries
using kernelFunction::kernelLambda;


template<Arithmetic T>
class ml_KDE_array : multi_levelKDE<T>{
private:
    const long double epsilon; //maybe not const??
    const KDE<T> kde[];

public:
    ml_KDE_array
    (
        list<vector<T>>* data,
        long double epsilon,
        double sigma, //might need to store sigma
        const kernelLambda<T> kernelLambda
    ){
        this->epsilon = epsilon;
        kde = new KDE<T>[pow(2, log(data->size())+1)];
        auto* dataArray = new vector<T>[data->size()];
        rec_construct(dataArray, dataArray->size(), kernelLambda, sigma, 1);
    }

private:
    void rec_construct
    (
        vector<T>* data,
        int dataSize,
        int startPoint,
        int endPoint,
        const kernelLambda<T> kernelLambda,
        double sigma,
        int arrayIndex
    ){
        //check if done.
        if (dataSize < 2) return;

        //creates this layers KDE
        kde[arrayIndex] = new KDE_exact<T>(kernelLambda,data,sigma);

        //computes the middle and
        int m = (startPoint + endPoint) / 2;
        //recursive left child
        rec_construct(data,startPoint,m,kernelLambda,sigma,arrayIndex*2);
        //recursive right child
        rec_construct(data,m+1,endPoint,kernelLambda,sigma,arrayIndex*2+1);
    }
};
#endif //KDE_SUB_QUADRATIC_ML_KDE_ARRAY_H
