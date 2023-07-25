#pragma once

#include <memory>

#include "User.h"

namespace ir
{
  class BasicBlock;
  class Instruction : public User
  {
  public:
    Instruction(std::shared_ptr<Type> type, std::string name, unsigned numOps)
        : User(std::move(type), name), numOps_(numOps){};
    Instruction(std::shared_ptr<Type> type, std::string name, unsigned numOps, std::shared_ptr<BasicBlock> parent) : User(std::move(type), name), numOps_(numOps), parent_(parent){};

    void setParent(std::shared_ptr<BasicBlock> parent) { parent_ = parent; };
    std::shared_ptr<BasicBlock> getParent() { return parent_; };

    bool isTerminator() { return is_terminate_inst_; };

  protected:
    unsigned numOps_;
    std::shared_ptr<BasicBlock> parent_;
    bool is_terminate_inst_ = false;
    std::vector<Use> operands_ = std::vector<Use>();
  };
}