#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Use.h"
#include "Value.h"

namespace ir
{
  class User : public Value
  {
  public:
    explicit User(std::unique_ptr<Type> type, std::string name = "") : Value(std::move(type), std::move(name)){};
  };
}
