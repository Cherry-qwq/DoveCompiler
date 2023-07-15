#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Value.h"

namespace ir
{
  class User : public Value
  {
  public:
    explicit User(std::shared_ptr<Type> type, std::string name = "") : Value(type, name){};
  };
}
