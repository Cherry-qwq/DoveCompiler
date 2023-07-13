#pragma once

#include <memory>

namespace ir
{
  class Value;
  class User;
  class Use
  {
  public:
    Use(std::shared_ptr<Value> val, User *user) : val_(val), user_(user) {}
    std::shared_ptr<Value> getValue() const { return val_; }
    void setValue(std::shared_ptr<Value> val) { val_ = val; }

    User *getUser() const { return user_; }
    void setUser(User *user) { user_ = user; }

  protected:
    std::shared_ptr<Value> val_;
    User *user_;
  };
}
