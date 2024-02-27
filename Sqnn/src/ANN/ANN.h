//
// Created by cj on 26-2-24.
//

#ifndef KDE_SUB_QUADRATIC_ANN_H
#define KDE_SUB_QUADRATIC_ANN_H

#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

template<class T>
class Ann {
private:
public:
	Ann(std::vector<std::vector<T>> data, const int dimension) {
		//int nn = 3;
		//flann::Matrix<float> dataset;
		//flann::Matrix<float> query;
		//flann::load_from_file(dataset, "dataset.hdf5","dataset");
		//flann::load_from_file(query, "dataset.hdf5","query");
		//flann::Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
		//flann::Matrix<float> dists(new float[query.rows*nn], query.rows, nn);
// construct an randomized kd-tree index using 4 kd-trees
		//flann::Index<flann::L2<float> > index(dataset, flann::KDTreeIndexParams(4));
		//index.buildIndex();
// do a knn search, using 128 checks
		//index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));
		//flann::save_to_file(indices,"result.hdf5","result");
	}

};

#endif //KDE_SUB_QUADRATIC_ANN_H
