#include "consts_reader.h"

#include <fstream>
#include <iostream>

#include "../../json/single_include/nlohmann/json.hpp"

MagicConsts::MagicConsts() {}

void MagicConsts::readJson() {
  std::string fName = "../resources/const.json";
  std::ifstream fileStream(fName);
  if (!fileStream.is_open()) {
    fName = "resources/const.json";
    fileStream.open(fName);
  }

  if (fileStream.is_open()) {
    nlohmann::json jsonMain1;
    try {
      fileStream >> jsonMain1;
    } catch (...) {
      std::cerr << "exception on reading " << fName << std::endl;
    }
    for (auto iter = jsonMain1.begin(); iter != jsonMain1.end(); ++iter) {
      m_consts[iter.key()] = iter.value().get<float>();
    }
  } else {
    throw "file not open fName = " + fName;
  }
}

float MagicConsts::getValue(std::string&& name) { return m_consts.at(name); }
