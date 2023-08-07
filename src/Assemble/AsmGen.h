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



    void genCompUnit(std::shared_ptr<ir::CompUnit> node);

    void generate();
    std::string exportToString();

    void exportToFile(std::string path);
  protected:
    std::shared_ptr<ir::CompUnit> comp_unit_;
    std::stringstream s_;
  };
}