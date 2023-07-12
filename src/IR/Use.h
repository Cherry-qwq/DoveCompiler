#pragma once

#include <memory>

#include "Value.h"
#include "User.h"

namespace ir
{
  class Use
  {
  public:
    Use(std::shared_ptr<Value> val, User *user) : val_(val), user_(user) {}
    std::shared_ptr<Value> getValue() const { return val_; }
    User *getUser() const { return user_; }
    void setValue(std::shared_ptr<Value> val) { val_ = val; }
    void setUser(User *user) { user_ = user; }

  protected:
    std::shared_ptr<Value> val_;
    User *user_;
  };
}
