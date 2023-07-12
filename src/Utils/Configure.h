#pragma once

#include <iostream>
#include <string>
#include <optional>

namespace config
{
  struct Config
  {
    std::string src_file_path;
    std::string dst_file_path;
    friend std::ostream &operator<<(std::ostream &os, const Config &c)
    {
      os << "Configure : {" << std::endl;
      os << "\tsrcFilePath : " << c.src_file_path << std::endl;
      os << "\tdstFilePath : " << c.dst_file_path << std::endl;
      os << "}" << std::endl;
      return os;
    }
  };
  Config &init(int argc, char **argv);
  Config &get();
}