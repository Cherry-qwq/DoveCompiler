#include "Use.h"
#include "Value.h"

namespace ir
{
  class User : public Value
  {
    Use *OperandList;
    unsigned NumOperands;

  public:
    User(PrimitiveDataType type, unsigned numOperands);
    virtual ~User();

    unsigned getNumOperands() const { return NumOperands; }
    Use *getOperandList() const { return OperandList; }
  };
}
