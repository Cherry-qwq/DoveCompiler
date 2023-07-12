#pragma once

#include <string>
#include <memory>


#include "Type.h"
#include "Value.h"


namespace ir
{
  class DumpHelper
  {
  public:
    std::string dump(const Value &value){
      Value *v = const_cast<Value *>(&value);
      return v->dump(*this);
    };
  };
}