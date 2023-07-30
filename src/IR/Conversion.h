#pragma once

#include "Instruction.h"

namespace ir
{
  class ZExt : public Instruction
  {
  public:
    ZExt(std::shared_ptr<Value> src, std::shared_ptr<Value> dst, std::shared_ptr<Type> targetType)
        : Instruction(targetType, src->getName(), 1), src_(src, this), targetType_(targetType){};

  protected:
    Use src_;
    std::shared_ptr<Type> targetType_;
  };
}