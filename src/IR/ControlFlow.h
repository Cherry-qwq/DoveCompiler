#pragma once

#include <memory>
#include <vector>
#include <string>

#include "BasicBlock.h"
#include "Instruction.h"
namespace ir
{

  class CompUnit : public User
  {
  public:
    CompUnit(std::shared_ptr<Type> type, std::string name)
        : User(std::move(type), name){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "CompUnit " + getName() + " ";
      helper.add(output);
      helper.indent();
      for (auto global_object : global_objects_)
      {
        global_object.getValue()->dump(helper);
      }
      helper.unindent();
      return output;
    }

    void addGlobalObject(std::shared_ptr<User> global_object)
    {
      global_objects_.push_back(Use(std::move(global_object), this));
    }

    std::shared_ptr<std::vector<Use>> getGlobalObject()
    {
      return std::make_shared<std::vector<Use>>(global_objects_);
    }

  protected:
    std::vector<Use> global_objects_;
  };

  class Function : public User
  {
  public:
    Function(std::shared_ptr<Type> type, std::string name)
        : User(std::move(type), std::move(name)){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Function " + getName() + ": " + getType()->dump();
      helper.add(output);
      helper.indent();
      for (auto basic_block : basic_blocks_)
      {
        basic_block->dump(helper);
      }
      helper.unindent();
      return output;
    }

    void addBasicBlock(std::shared_ptr<BasicBlock> basic_block)
    {
      basic_blocks_.push_back(std::move(basic_block));
    }

    std::shared_ptr<std::vector<std::shared_ptr<BasicBlock>>> getBasicBlocks()
    {
      return std::make_shared<std::vector<std::shared_ptr<BasicBlock>>>(basic_blocks_);
    }

  protected:
    std::vector<std::shared_ptr<BasicBlock>> basic_blocks_;
  };

  class Icmp : public Instruction
  {
    // TODO
  };

  class Branch : public Instruction
  {
  public:
    Branch(std::shared_ptr<Value> condition, std::string name)
        : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 3), condition_(condition, this){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Branch " + getName() + " ";
      helper.add(output);
      helper.indent();
      helper.add(condition_.getValue()->dump(helper));
      helper.unindent();
      return output;
    }
    void setCondition(std::shared_ptr<Value> condition)
    {
      condition_ = Use(condition, this);
    }
    std::shared_ptr<Value> getCondition()
    {
      return condition_.getValue();
    }
    void setTrueTarget(std::shared_ptr<BasicBlock> true_target)
    {
      true_target_ = true_target;
    }
    std::shared_ptr<BasicBlock> getTrueTarget(std::shared_ptr<BasicBlock> true_target)
    {
      return true_target_;
    }
    void setFalseTarget(std::shared_ptr<BasicBlock> false_target)
    {
      false_target_ = false_target;
    }
    std::shared_ptr<BasicBlock> getFalseTarget(std::shared_ptr<BasicBlock> false_target)
    {
      return false_target_;
    }

  protected:
    std::shared_ptr<BasicBlock> true_target_;
    std::shared_ptr<BasicBlock> false_target_;
    Use condition_;
  };

  class Jump : public Instruction // Used in Break and Continue
  {
  public:
    Jump(std::shared_ptr<BasicBlock> target, std::string name)
        : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 1), target_(target){};
    void setTarget(std::shared_ptr<BasicBlock> target)
    {
      target_ = target;
    }
    std::shared_ptr<BasicBlock> getTarget()
    {
      return target_;
    }

  protected:
    std::shared_ptr<BasicBlock> target_;
  };

  class Return : public Instruction
  {
  public:
    Return(std::shared_ptr<Value> retVal, std::string name)
        : Instruction(std::move(retVal->getType()), std::move(name), 1), ret_val_(retVal, this){};

  protected:
    Use ret_val_;
  };

  class Phi : public Instruction
  {
    // TODO
  };

  class Call : public Instruction
  {
    // TODO
  };
}
