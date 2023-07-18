
#include "Type.h"

#include <memory>

namespace ir
{
  std::shared_ptr<PrimitiveDataType> MakePrimitiveDataType(PrimitiveDataType::TypeID id)
  {
    return std::make_shared<PrimitiveDataType>(id);
  }

  std::shared_ptr<PointerType> MakePointerType(std::shared_ptr<Type> ref)
  {
    return std::make_shared<PointerType>(ref);
  }

  std::shared_ptr<Type> MakeType(std::shared_ptr<Type> ptr)
  {
    // moe_assert(dynamic_cast<PointerType *>(pointer.get()));
    return ptr->get();
  }

  std::shared_ptr<ArrayType> MakeArrayType(std::shared_ptr<Type> elementType,size_t len)
  {
    return std::make_shared<ArrayType>(elementType,len);
  }
}