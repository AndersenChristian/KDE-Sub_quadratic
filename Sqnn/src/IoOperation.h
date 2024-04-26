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

  inline std::vector<std::string> SplitString(const std::string &str) {
    std::istringstream iss(str);
    std::vector<std::string> result;
    for (std::string s; iss >> s;) {
      result.push_back(s);
    }
    return result;
  }

  inline bool LoadData(const std::string &FILENAME, int &n, int &d, float &rho, float &h, Eigen::MatrixXf &X) {
    // Open the file
    std::ifstream file(FILENAME);

    // Check if the file opened successfully
    if (file.is_open()) {
      //TODO: loop and safe.
      //TODO: maybe parallel? not sure yet, or if that could cause mem issue
      std::string line;
      std::stringstream ss(line);

      std::getline(file, line);
      std::vector<std::string> data = SplitString(line);
      n = std::stoi(data[0]);
      d = std::stoi(data[1]);
      rho = std::stof(data[2]);
      h = std::stof(data[3]);

      X = Eigen::MatrixXf(d, n);

      for (int i = 0; i < n; ++i) {
        std::getline(file, line);
        data = SplitString(line);
        for (int j = 0; j < d; ++j)
          X(j, i) = std::stof(data[j]);
      }
      file.close();
      return true;
    } else {
      printf("couldn't find file.\nshutdown...");
      return false;
    }
  }
}

#endif //KDE_SUB_QUADRATIC_IOOPERATION_H
