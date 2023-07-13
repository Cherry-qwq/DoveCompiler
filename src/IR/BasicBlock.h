#pragma once

#include <memory>
#include <string>

#include "Instruction.h"

namespace ir
{
  class BasicBlock : public Value
  {
  public:
    explicit BasicBlock(std::string name) : Value(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name)){};
    std::string dump(DumpHelper &helper) const override
    {
      auto s = "BasicBlock " + getName();
      for (auto &inst : instructions_)
      {
        s += "\n\t" + inst->dump(helper);
      }
      return s;
    };
    
    void addInstruction(std::shared_ptr<Instruction> inst)
    {
      inst->setParent(std::make_shared<BasicBlock>(*this));
      instructions_.push_back(std::move(inst));
    };

  protected:
    std::vector<std::shared_ptr<Instruction>> instructions_;
  };
}