#pragma once

#include <memory>

#include "Type.h"
#include "Instruction.h"

namespace ir
{
  class ConstValue : public Value
  {
  public:
    explicit ConstValue(std::shared_ptr<Type> type, std::string name = "") : Value(std::move(type), name){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "ConstValue ";
      helper.add(output);
      return output;
    }
  };
}