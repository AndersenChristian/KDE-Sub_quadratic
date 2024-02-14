//
// Created by cj on 6-2-24.
//

#include "KDE.h"
#include <algorithm>
#include <iostream>

using std::all_of;

template<Arithmetic T>
class KDE_scikit : KDE<T>{
    T data[];

    KDE_scikit(list<vector<T>> data){
        int size = data.front().size();
        for(const vector<int>& d : data){
            if(size != d.size())
                throw std::invalid_argument("wrong data size");
        }



    }

};

template class KDE_scikit<int>;