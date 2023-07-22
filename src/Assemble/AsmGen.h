#pragma once

#include <memory>
#include <string>
#include <sstream>

#include "IR/ControlFlow.h"

namespace asmgen
{
  class AsmGenerator
  {
  public:
    AsmGenerator(std::shared_ptr<ir::CompUnit> comp_unit) : comp_unit_(comp_unit){};
    ~AsmGenerator() = default;

    std::string generate();

    void exportToFile(std::string path);
  protected:
    std::shared_ptr<ir::CompUnit> comp_unit_;
    std::stringstream s_;
  };
}