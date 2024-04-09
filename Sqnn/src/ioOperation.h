//
// Created by cj on 9-4-24.
//

#ifndef KDE_SUB_QUADRATIC_IOOPERATION_H
#define KDE_SUB_QUADRATIC_IOOPERATION_H

#include <filesystem>
#include <fstream>
#include <string>
#include <Eigen/Dense>

namespace io {

	inline std::vector<std::string> splitString(const std::string &str) {
		std::istringstream iss(str);
		std::vector<std::string> result;
		for (std::string s; iss >> s;) {
			result.push_back(s);
		}
		return result;
	}

	inline bool loadData(const std::string &filename, int &n, int &d, Eigen::MatrixXf &X, Eigen::VectorXf &q) {
		// Open the file
		std::ifstream file(filename);

		// Check if the file opened successfully
		if (file.is_open()) {
			//TODO: loop and safe.
			//TODO: maybe parallel? not sure yet, or if that could cause mem issue
			std::string line;
			std::stringstream ss(line);

			std::getline(file, line);
			std::vector<std::string> data = splitString(line);
			n = std::stoi(data[0]);
			d = std::stoi(data[1]);

			X = Eigen::MatrixXf(d, n);
			q = Eigen::VectorXf(d);

			for (int i = 0; i < n; ++i) {
				std::getline(file, line);
				data = splitString(line);
				for (int j = 0; j < d; ++j)
					X(j, i) = std::stof(data[j]);
			}
			for (int i = 0; i < d; ++i) q(i) = 0;
			file.close();
			return true;
		} else {
			printf("couldn't find file.\nshutdown...");
			return false;
		}
	}
}

#endif //KDE_SUB_QUADRATIC_IOOPERATION_H
