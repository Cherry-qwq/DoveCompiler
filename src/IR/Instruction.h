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
    void setSTRegister(bool request_register) { request_register_ = request_register; };
    bool getSTRegister() { return request_register_; };
    void setRegName(std::string reg_name) { reg_name_ = reg_name; };
    std::string getRegName() { return reg_name_; };

  protected:
    unsigned numOps_;
    std::shared_ptr<BasicBlock> parent_;
    bool request_register_ = true;
    std::string reg_name_;
  };
}