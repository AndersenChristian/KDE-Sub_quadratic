//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "src/API/kernelFunction.h"
#include "ENUM/kernelType.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using std::function;
using std::vector;
using std::string;
using std::stoi;

//predecleration
kernelType getKernelType(const char&);


/**
 *
 *
 * @param argc number of data or number of argv
 * @param argv contains the dataType and the data
 *             argv[0] path to directory, argv[1] data type, argv[2] kernel type, argv[3] dimension and argv[4..n] data
 * @return
 */
int main(int argc, char *argv[]){

    //ensures that there is at least 1 value.
    if (argc < 4) return 1;

    //checks the kernel type
    if(std::strlen(argv[2]) != 1)
        return 3;
    try {
        const kernelType kernelType = getKernelType(argv[2][0]);
    } catch (std::exception &e){
        std::cerr << e.what();
        return 3;
    }

    //build data structure
    const unsigned int dimension = stoi(argv[3]);
    const unsigned int dataSize = (argc - 3) / dimension;
    const char *dataType = argv[1];
    //const kernelType kernelType = getKernelType(argv[2][0]);

    if(string(dataType) == "double"){
        auto data = vector<vector<double>>(dataSize, vector<double>(dimension));
    } else {
        return 2;
    }


    std::cout << argc << "\n";

    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << " ";
    }



    const auto kernel_function =
            kernelFunction::kernel_function<double>(kernelType::Gaussian);

    vector<vector<double>> vectors = {{
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

    //tear down
    delete kde;

    return 0;
}

inline kernelType getKernelType(const char& kernelType) {
    switch (kernelType) {
        case 'G':
            return kernelType::Gaussian;
        case 'E':
            return kernelType::Exponential;
        case 'L':
            return kernelType::Laplacian;
        default:
            throw std::invalid_argument("kernel type only accept the letter G, E and L");
    }
}