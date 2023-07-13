
#include "Type.h"

#include <memory>

namespace ir
{
  std::unique_ptr<PrimitiveDataType> MakePrimitiveDataType(PrimitiveDataType::TypeID id)
  {
    return std::make_unique<PrimitiveDataType>(id);
  }

  std::unique_ptr<PointerType> MakePointerType(std::unique_ptr<Type> ref)
  {
    return std::make_unique<PointerType>(std::move(ref));
  }

  std::unique_ptr<Type> MakeType(std::unique_ptr<Type> ptr)
  {
    // moe_assert(dynamic_cast<PointerType *>(pointer.get()));
    auto p = std::unique_ptr<PointerType>(dynamic_cast<PointerType *>(ptr.release()));
    return p->get();
  }

}