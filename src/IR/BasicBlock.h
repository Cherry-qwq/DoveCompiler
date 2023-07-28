#pragma once

#include <memory>
#include <optional>
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
      WhileBody

    };

    explicit BasicBlock(const std::string &name) : BasicBlock(name, BlockType::FunctionEntry){};
    explicit BasicBlock(const std::string &name, BlockType type) : BasicBlock(std::make_shared<JPLabel>(name), type){};
    explicit BasicBlock(std::shared_ptr<JPLabel> jplabel, BlockType type) : User(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), jplabel->getName()), jplabel_(jplabel), block_type_(type){};
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
      case BlockType::WhileBody:
        return "WhileBody";
      }
      return "Unknown";
    };
    std::string getLabel = jplabel_;
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

    std::optional<std::shared_ptr<Instruction>> getTerminator()
    {
      if (instructions_.empty())
      {
        return std::nullopt;
      }
      auto last = std::dynamic_pointer_cast<Instruction>(instructions_.back().getValue());
      if (last->isTerminator())
      {
        return last;
      }
      else
      {
        return std::nullopt;
      }
    }

  protected:
    std::shared_ptr<JPLabel> jplabel_;
    std::vector<Use> instructions_;
    BlockType block_type_;
  };
}