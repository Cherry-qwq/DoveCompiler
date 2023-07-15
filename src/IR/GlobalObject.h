#pragma once

#include "User.h"


namespace ir
{
  class GlobalObject : public User
  {
  public:
    GlobalObject(std::shared_ptr<Type> type, std::string name)
        : User(std::move(type), std::move(name)){};
    virtual ~GlobalObject() = default;
  };
}