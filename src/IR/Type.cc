
#include "Type.h"

namespace IR
{

  PrimitiveDataType::PrimitiveDataType(TypeID id) : type_(id) {}
  std::unique_ptr<Type> PrimitiveDataType::get() const
  {
    return nullptr;
  }
  std::string PrimitiveDataType::dump()
  {
    switch (type_)
    {
    case TypeID::Int32:
      return "Int32";
    case TypeID::Float32:
      return "Float32";
    case TypeID::Boolean:
      return "Boolean";
    case TypeID::Void:
      return "Void";
    default:
      return "Unknown";
    }
  }
  std::unique_ptr<PrimitiveDataType> makePrimitiveDataType(PrimitiveDataType::TypeID id)
  {
    return std::make_unique<PrimitiveDataType>(id);
  }


  PointerType::PointerType(std::unique_ptr<Type> ref) : ref_(std::move(ref)) {}
  PointerType::PointerType(const PointerType &other) : ref_(other.ref_->get()) {}
  std::unique_ptr<Type> PointerType::get() const
  {
    return ref_->get();
  }
  std::string PointerType::dump()
  {
    return "Pointer(" + ref_->dump() + ")";
  }
  std::unique_ptr<PointerType> makePointer(std::unique_ptr<Type> ref)
  {
    return std::make_unique<PointerType>(std::move(ref));
  }

  std::unique_ptr<Type> makeType(std::unique_ptr<Type> ptr)
  {
    // moe_assert(dynamic_cast<PointerType *>(pointer.get()));
    auto p = std::unique_ptr<PointerType>(dynamic_cast<PointerType *>(ptr.release()));
    return p->get();
  }
}