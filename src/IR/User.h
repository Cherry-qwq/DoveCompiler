#include "Use.h"
#include "Value.h"

namespace ir
{
  class User : public Value
  {
    Use *OperandList;
    unsigned NumOperands;

  public:
    User(std::unique_ptr<Type> type, unsigned numOperands);

    unsigned getNumOperands() const { return NumOperands; }
    Use *getOperandList() const { return OperandList; }
  };
}
