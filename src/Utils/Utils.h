#pragma once

#include <string>

namespace utils
{
  class Utils
  {
  };

  int32_t ParseInt32(const std::string &str)
  {
    return std::stoi(str);
  }
  float ParseFloat32(const std::string &str)
  {
    return std::stof(str);
  }

}