#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "Type.h"
#include "Instruction.h"

namespace ir
{
  
  class Load : public Instruction
  {
  public:
    Load(std::shared_ptr<Value> ptr, std::string name) : Instruction(ptr->getType(), std::move(name), 1), ptr_(ptr, this){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Load " + ptr_.getValue()->dump(helper);
      helper.add(output);
      return output;
    }

  protected:
    Use ptr_;
  };

  class Store : public Instruction
  {
  public:
    Store(std::shared_ptr<Value> val, std::shared_ptr<Value> ptr) : Instruction(MakePrimitiveDataType(PrimitiveDataType::TypeID::Void), "Store", 2), val_(val, this), ptr_(ptr, this){};

    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Store " + val_.getValue()->dump(helper) + " " + ptr_.getValue()->dump(helper);
      helper.add(output);
      return output;
    }

  protected:
    Use val_;
    Use ptr_;
  };

  class GetElementPtr : public Instruction
  {
  public:
    explicit GetElementPtr(std::shared_ptr<User> ptr, std::shared_ptr<Type> type, std::vector<std::shared_ptr<User>> idx, const std::string &name) : Instruction(MakePointerType(ptr->getType()->copy()), std::move(name), 2), ptr_(ptr, this){
      for (auto &i : idx)
      {
        idx_.push_back(Use(i, this));
      }
    };

  protected:
    Use ptr_;
    std::vector<Use> idx_;
  };

  class ExtractValue : public Instruction
  {
  };

  class InsertValue : public Instruction
  {
  };
}
