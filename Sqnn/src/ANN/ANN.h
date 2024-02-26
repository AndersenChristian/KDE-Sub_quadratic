//
// Created by cj on 26-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ANN_H
#define KDE_SUB_QUADRATIC_ANN_H

#include "NGT/Index.h"

template <class T>
class Ann {
private:
    NGT::Index mIndex;
    NGT::Property mProperty;
public:
    Ann(std::vector<std::vector<T>> data, const int dimension){
        mProperty.dimension = dimension;
        //Todo, switch case i guess on types?
        mProperty.objectType = NGT::ObjectSpace::ObjectType::Float;
        //Todo, do we always use L2? and does that mean second norm
        mProperty.distanceType = NGT::Index::Property::DistanceType::DistanceTypeL2;

        //build index:
        std::string   path("anng");
        NGT::Index::create(path, mProperty);
        NGT::Index    index(path);
        for(const auto x : data){
            index.append(x);
        }
        index.createIndex(16);
        index.save();
    }

    void NNS(std::vector<T> point){
        if (point.size() != mProperty.dimension) throw std::invalid_argument("size of vector must be equal to the init vector");
        std::cout << "test";
    }
};

#endif //KDE_SUB_QUADRATIC_ANN_H
