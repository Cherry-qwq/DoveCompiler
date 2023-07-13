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
    virtual std::unique_ptr<Type> copy()
    {
      return std::unique_ptr<Type>(this);
    };
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
    std::unique_ptr<Type> copy() const
    {
      return std::make_unique<PrimitiveDataType>(type_);
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
  std::unique_ptr<PrimitiveDataType> MakePrimitiveDataType(PrimitiveDataType::TypeID id);

  class PointerType : public Type
  {
  public:
    explicit PointerType(std::unique_ptr<Type> ref) : ref_(std::move(ref)) {}
    PointerType(const PointerType &other) : ref_(other.ref_->get()) {}
    std::unique_ptr<Type> get() const
    {
      return ref_->get();
    }
    std::unique_ptr<Type> copy() const
    {
      return std::make_unique<PointerType>(ref_->copy());
    }
    std::string dump()
    {
      return "Pointer(" + ref_->dump() + ")";
    }

  protected:
    std::unique_ptr<Type> ref_;
  };
  std::unique_ptr<PointerType> MakePointerType(std::unique_ptr<Type> ref);

  std::unique_ptr<Type> MakeType(std::unique_ptr<Type> ptr);
}
