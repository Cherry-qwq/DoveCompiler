#pragma once
#include <memory>
#include <string>

namespace ir
{
  class Type
  {
  public:
    virtual ~Type() = default;
    virtual std::unique_ptr<Type> get() const = 0;
    virtual std::string dump() = 0;
  };

  class PrimitiveDataType : public Type
  {
  public:
    enum class TypeID
    {
      Int32,
      Float32,
      Boolean,
      Void,
    };

    PrimitiveDataType(TypeID id) : type_(id) {}
    std::unique_ptr<Type> get() const
    {
      return nullptr;
    }
    std::string dump()
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

  protected:
    TypeID type_;
  };
  std::unique_ptr<PrimitiveDataType> MakePrimitiveDataType(PrimitiveDataType::TypeID id)
  {
    return std::make_unique<PrimitiveDataType>(id);
  }

  class PointerType : public Type
  {
  public:
    explicit PointerType(std::unique_ptr<Type> ref) : ref_(std::move(ref)) {}
    PointerType(const PointerType &other) : ref_(other.ref_->get()) {}
    std::unique_ptr<Type> PointerType::get() const
    {
      return ref_->get();
    }
    std::string PointerType::dump()
    {
      return "Pointer(" + ref_->dump() + ")";
    }

  protected:
    std::unique_ptr<Type> ref_;
  };
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
