//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "src/API/kernelFunction.h"
#include "ENUM/kernelType.h"

#include <iostream>
#include <vector>
#include <algorithm>

using std::function;
using std::vector;
using std::string;

int main(int argc, char *argv[]){

    const auto kernel_function =
            kernelFunction::kernel_function<double>(kernelType::Gaussian);

    vector<vector<double>> vectors = {{
        4.0
    }};

    unsigned int start = 0, end = 1;
    KDE<double>* kde = new KDE_exact<double>(kernel_function, vectors, start, end, 2.0);
    double s = kde->QueryNewPoint({4.0});
    std::cout << s << "\n";


    //computes sum
    double sum = 0.0;
    for(int i = -1000; i < 1000; i ++){
        sum += kde->QueryNewPoint({(double) i});
    }

    std::cout << sum;



    //tear down
    delete kde;

    // Clear all vectors and deallocate memory
    std::for_each(vectors.begin(), vectors.end(), [](std::vector<double>& inner_vec) {
        inner_vec.clear();
    });

    // Clear the outer vector
    vectors.clear();

    return 0;
}