#pragma once

#include "User.h"


namespace ir
{
  class GlobalObject : public User
  {
  public:
    GlobalObject() = default;
    virtual ~GlobalObject() = default;
  };
}