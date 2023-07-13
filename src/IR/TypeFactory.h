#pragma once


#include <memory>



namespace ir
{
  class Type;
  class TypeFactory
  {
  };
  // std::unique_ptr<Type> CopyType(const std::unique_ptr<Type> &type)
  // {
  //   auto *p = type.get();
  //   return std::unique_ptr<Type>(p->get());
  // }
}
