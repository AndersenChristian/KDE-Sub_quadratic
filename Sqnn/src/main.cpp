//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "src/API/kernelFunction.h"
#include "ENUM/kernelType.h"

#include <iostream>
#include <vector>

using std::function;
using std::vector;

int main(int argc, char *argv[]){

    const auto kernel_function =
            kernelFunction::kernel_function<double>(kernelType::Gaussian);

    std::list<vector<double>> vectors = {{
        4.0
    }};

    KDE<double>* kde = new KDE_exact<double>(kernel_function, vectors, 2.0);
    double s = kde->QueryNewPoint({4.0});
    std::cout << s << "\n";


    //computes sum
    double sum = 0.0;
    for(int i = -1000; i < 1000; i ++){
        sum += kde->QueryNewPoint({(double) i});
    }

    std::cout << sum;


    return 0;
}