#pragma once

#include <memory>

#include "Type.h"


namespace ir
{
  class TypeFactory
  {
  };
  std::unique_ptr<Type> copyType(const std::unique_ptr<Type> &type)
  {
    auto *p = type.get();
    return std::unique_ptr<Type>(p->get());
  }
}