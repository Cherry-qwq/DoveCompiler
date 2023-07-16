#pragma once

#include <memory>
#include <vector>
#include <string>

#include "User.h"
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

  class Icmp : public User
  {
    // TODO
  };

  class Branch : public User
  {
  public:
    Branch(std::shared_ptr<Value> condition, std::string name)
        : User(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name)), condition_(condition, this){};
    void setCmpResult(std::shared_ptr<Value> condition)
    {
      condition_ = Use(condition, this);
    }
    std::shared_ptr<Value> getCmpResult()
    {
      return condition_.getValue();
    }
    void setTrueBranch(std::shared_ptr<BasicBlock> true_branch)
    {
      true_branch_ = true_branch;
    }
    std::shared_ptr<BasicBlock> getTrueBranch(std::shared_ptr<BasicBlock> true_branch)
    {
      return true_branch_;
    }
    void setFalseBranch(std::shared_ptr<BasicBlock> false_branch)
    {
      false_branch_ = false_branch;
    }
    std::shared_ptr<BasicBlock> getFalseBranch(std::shared_ptr<BasicBlock> false_branch)
    {
      return false_branch_;
    }

  protected:
    std::shared_ptr<BasicBlock> true_branch_;
    std::shared_ptr<BasicBlock> false_branch_;
    Use condition_;
  };

  class Jump : public Instruction
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

  class Return : public User
  {
  public:
    Return(std::shared_ptr<Value> retVal, std::string name)
        : User(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name)), ret_val_(retVal, this){};

  protected:
    Use ret_val_;
  };

  class Phi : public User
  {
    // TODO
  };
}
