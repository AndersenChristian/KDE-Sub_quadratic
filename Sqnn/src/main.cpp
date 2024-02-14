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
            ::kernel_function<double>(kernel::Gaussian);

    std::list<vector<double>> vectors = {{
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
       {3.0, 3.0, 3.0},
    }};

    KDE<double>* kde = new KDE_exact<double>(kernel_function, vectors);
    double s = kde->QueryNewPoint({2.0,3.0,3.0});

    std::cout << s;

    return 0;
}