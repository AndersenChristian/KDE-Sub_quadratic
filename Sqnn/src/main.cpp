//
// Created by cj on 6-2-24.
//

#include "KDE/KDE_exact.h"
#include "src/API/kernelFunction.h"
#include "ENUM/kernelType.h"
#include "KDE/ml-KDE_array.h"

#include <iostream>
#include <vector>
#include <algorithm>

using std::function;
using std::vector;
using std::string;

int main(int argc, char *argv[]){

    vector<vector<double>> vectors = {
            {4.0},
            {4.0},
            {4.0},
            {4.0},
            {2.0},
            {2.0},
            {2.0},
            {2.0}
    };

    long double epsilon = 1;
    double sigma = 2;
    auto mlKDE = new ml_KDE_array<double>(&vectors, epsilon, sigma, Gaussian);

    auto kdeRoot = mlKDE->getKDE();
    auto kdeElement = &kdeRoot[1];
    std::cout << (*kdeElement)->QueryNewPoint({2.0}) << "\n";

    unsigned int currentPosition = kdeElement - kdeRoot;
    unsigned int nextPosition = currentPosition * 2;
    kdeElement = kdeElement + nextPosition - 1;

    std::cout << (*kdeElement)->QueryNewPoint({2.0}) << "\n";
    std::cout << (*(kdeElement + 1))->QueryNewPoint({2.0}) << "\n";

    //std::cout << kde.second->QueryNewPoint({3.0});

    //std::cout << "KDE:" << sizeof(KDE<double>) << "\tKDE_EXACT:" << sizeof(KDE_exact<double>);



    //tear down \
    delete kde;

    // Clear all vectors and deallocate memory
    std::for_each(vectors.begin(), vectors.end(), [](std::vector<double>& inner_vec) {
        inner_vec.clear();
    });

    // Clear the outer vector
    vectors.clear();

    return 0;
}