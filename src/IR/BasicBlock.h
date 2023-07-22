#pragma once

#include <memory>
#include <string>

#include "Instruction.h"

namespace ir
{
  class BasicBlock : public User, std::enable_shared_from_this<BasicBlock>
  {
  public:
    enum class BlockType
    {
      Default,
      FunctionEntry,
      IfTrue,
      IfFalse,
      WhileEntry,

    };

    explicit BasicBlock(const std::string &name) : BasicBlock(name, BlockType::FunctionEntry){};
    explicit BasicBlock(const std::string &name, BlockType type) : User(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name)), block_type_(type){};
    std::string getStringifyBlockType() const
    {
      switch (block_type_)
      {
      case BlockType::Default:
        return "Default";
      case BlockType::FunctionEntry:
        return "FunctionEntry";
      case BlockType::IfTrue:
        return "IfTrue";
      case BlockType::IfFalse:
        return "IfFalse";
      case BlockType::WhileEntry:
        return "WhileEntry";
      }
      return "Unknown";
    };
    std::string getLabel = "label_" + getName();

    std::string dump(DumpHelper &helper) const override
    {
      auto s = "BasicBlock " + getName() + ": " + getStringifyBlockType();
      helper.add(s);
      helper.indent();
      for (auto &inst : instructions_)
      {
        helper.add(inst.getValue()->dump(helper));
      }
      helper.unindent();
      return s;
    };

    void addInstruction(std::shared_ptr<Instruction> inst)
    {
      inst->setParent(shared_from_this());
      instructions_.push_back(std::move(Use(inst, this)));
    };
    std::vector<Use> getInstructions() const
    {
      return instructions_;
    }

    BlockType getBlockType() const
    {
      return block_type_;
    }

  protected:
    std::vector<Use> instructions_;
    BlockType block_type_;
  };
}