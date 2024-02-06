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

    std::array<std::vector<double>, 20> vectors = {{
       {36.0, 18.0, 0.0},
       {18.0, 13.0, 47.0},
       {19.0, 37.0, 13.0},
       {27.0, 31.0, 30.0},
       {28.0, 46.0, 36.0},
       {28.0, 34.0, 49.0},
       {15.0, 10.0, 32.0},
       {33.0, 1.0, 31.0},
       {35.0, 0.0, 48.0},
       {11.0, 19.0, 10.0},
       {41.0, 5.0, 28.0},
       {41.0, 25.0, 41.0},
       {41.0, 44.0, 29.0},
       {6.0, 21.0, 12.0},
       {38.0, 1.0, 9.0},
       {24.0, 31.0, 43.0},
       {25.0, 46.0, 6.0},
       {9.0, 31.0, 9.0},
       {40.0, 17.0, 11.0},
       {41.0, 30.0, 31.0},
    }};

    double sum = 0.0;

    for(int i = 0; i < 20; i++){
        for(int j = i + 1; j < 20; j++){
            sum += kernel_function(vectors[i], vectors[j]);
        }
    }

    std::cout << sum << "\n";
    std::cout << sum/20;


}