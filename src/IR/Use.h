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

    // Use &operator=(Use &other)
    // {
    //   if (this != &other)
    //   {
    //     if (val_)
    //     {
    //       val_->unbindUse(this);
    //     }
    //     val_ = other.val_;
    //     if (val_)
    //     {
    //       val_->bindUse(this);
    //     }
    //     user_ = other.user_;
    //   }
    //   return *this;
    // }

  protected:
    std::shared_ptr<Value> val_;
    User *user_;
  };
}
