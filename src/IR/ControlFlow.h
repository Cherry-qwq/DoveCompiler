#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

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
        : User(std::move(type), std::move(name)) { is_function_ = true; };
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

  class Return : public Instruction
  {
  public:
    Return(std::shared_ptr<Value> retVal, std::string name)
        : Instruction(std::move(retVal->getType()), std::move(name), 1), ret_val_(retVal, this)
    {
      is_terminate_inst_ = true;
    };

  protected:
    Use ret_val_;
  };

  class JPLabel : public Instruction
  {
  public:
    explicit JPLabel(std::string label_name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), label_name, 0), label_name_(label_name){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "JPLabel " + getName() + ": ";
      helper.add(output);
      return output;
    }

  protected:
    std::string label_name_;
  };

  class Label : public Instruction
  {
  public:
    explicit Label(std::shared_ptr<JPLabel> jplabel) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), jplabel->getName(), 0), jplabel_(jplabel){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Label " + getName() + ": ";
      helper.add(output);
      return output;
    }

  protected:
    std::shared_ptr<JPLabel> jplabel_;
  };

  class Phi : public Instruction
  {
    // TODO
  protected:
    std::map<std::shared_ptr<JPLabel>, std::shared_ptr<Value>> vecPair;
  };

  class Call : public Instruction
  {
  public:
    explicit Call(std::shared_ptr<Function> callee, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2), callee_(callee){};
    explicit Call(std::shared_ptr<Function> callee, std::vector<std::shared_ptr<Value>> args, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2), callee_(callee)
    {
      for (auto &arg : args)
      {
        args_.push_back(Use(arg, this));
      }
    };

  protected:
    std::shared_ptr<Function> callee_;
    std::vector<Use> args_ = std::vector<Use>();
  };

  class Br : public Instruction
  {
  public:
    Br(std::shared_ptr<Value> condition, Label t_label, Label f_label, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 1), condition_(condition, this), t_label_(t_label), f_label_(f_label)
    {
      is_terminate_inst_ = true;
    };
    Br(std::shared_ptr<Label> t_label, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 0), t_label_(t_label)
    {
      is_terminate_inst_ = true;
    };
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Branch " + getName() + " ";
      helper.add(output);
      helper.indent();
      helper.add(condition_.getValue()->dump(helper));
      helper.unindent();
      return output;
    }
    bool hasCondition()
    {
      return has_condition_;
    }
    void setCondition(std::shared_ptr<Value> condition)
    {
      condition_ = Use(condition, this);
      has_condition_ = true;
    }
    std::shared_ptr<Value> getCondition()
    {
      return condition_.getValue();
    }

    std::shared_ptr<Label> getTLabel()
    {
      return t_label_;
    }
    std::shared_ptr<Label> getFLabel()
    {
      return f_label_;
    }

  protected:
    std::shared_ptr<Label> t_label_;
    std::shared_ptr<Label> f_label_;

    bool has_condition_ = false;
    Use condition_;
  };

}
