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
}