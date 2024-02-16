//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "kernel.h"

#include <iostream>
#include <vector>

using std::function;
using std::vector;

int main(int argc, char *argv[]){

    auto kernel_function =
            kernel::kernel_function<double>(kernels::Gaussian);

    std::list<vector<double>> vectors = {{
        4.0
    }};

    KDE<double>* kde = new KDE_exact<double>(kernel_function, vectors, 4.0);
    double s = kde->QueryNewPoint({4.0});
    std::cout << s << "\n";


    //computes sum
    double sum = 0.0;
    for(int i = -20; i < 40; i ++){
        sum += kde->QueryNewPoint({(double) i});
    }

    std::cout << sum;


    return 0;
}