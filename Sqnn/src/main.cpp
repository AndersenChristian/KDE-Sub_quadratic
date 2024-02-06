//
// Created by cj on 6-2-24.
//

#include "main.h"
#include "kernel.h"

#include <iostream>
#include <vector>

int main(int argc, char *argv[]){
    std::function
        <double(std::vector<double>, std::vector<double>)>
        kernel_function =
            ::kernel_function<double>(kernel::Gaussian);

    std::cout << "test\n";

    std::vector<double> x = {1.0, 2.0, 3.0};
    std::vector<double> y = {4.0, 5.0, 6.0};

    std::cout << kernel_function(x,y);

}