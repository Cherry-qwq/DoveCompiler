#include "Type.h"

namespace IR
{
  class Value
  {
    PrimitiveDataType type_;

  public:
    Value(PrimitiveDataType type);
    virtual ~Value();

    PrimitiveDataType getType() const { return type_; }
  };
}
