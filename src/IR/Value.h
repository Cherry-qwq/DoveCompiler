#include "Type.h"

namespace IR
{
  class Value
  {
    Type type_;

  public:
    Value(Type type);
    virtual ~Value();

    Type getType() const { return type_; }
  };
}
