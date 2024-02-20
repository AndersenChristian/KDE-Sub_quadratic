//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "src/API/kernelFunction.h"
#include "ENUM/kernelType.h"

#include <iostream>
#include <vector>
#include <string>

using std::function;
using std::vector;
using std::string;

//predecleration
kernelType getKernelType(const string&);


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

    const char *dataType = argv[1];
    const kernelType kernelType = getKernelType(string(argv[2]));
    const unsigned int dataSize = argc - 3;

    if(string(dataType) == "double"){
        auto data =
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

inline kernelType getKernelType(const string& kernelType){
    if (kernelType == )
}