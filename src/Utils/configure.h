#pragma once

#include <iostream>
#include <string>
#include <optional>

namespace config
{
  struct Config
  {
    std::string srcFilePath;
    std::string dstFilePath;
    friend std::ostream &operator<<(std::ostream &os, const Config &c)
    {
      os << "Configure : {" << std::endl;
      os << "\tsrcFilePath : " << c.srcFilePath << std::endl;
      os << "\tdstFilePath : " << c.dstFilePath << std::endl;
      os << "}" << std::endl;
      return os;
    }
  };
  Config &init(int argc, char **argv);
  Config &get();
}