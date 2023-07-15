#pragma once

#include <memory>
#include <string>

namespace ir
{
  class Type : std::enable_shared_from_this<Type>
  {
  public:
    virtual ~Type() = default;
    virtual std::shared_ptr<Type> get() const = 0;
    virtual std::shared_ptr<Type> copy() const =0;
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
    std::shared_ptr<Type> get() const
    {
      return std::make_shared<PrimitiveDataType>(type_);
    }
    std::shared_ptr<Type> copy() const
    {
      return std::make_shared<PrimitiveDataType>(type_);
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
  std::shared_ptr<PrimitiveDataType> MakePrimitiveDataType(PrimitiveDataType::TypeID id);

  class PointerType : public Type
  {
  public:
    explicit PointerType(std::shared_ptr<Type> ref) : ref_(std::move(ref)) {}
    PointerType(const PointerType &other) : ref_(other.ref_->get()) {}
    std::shared_ptr<Type> get() const
    {
      return ref_->get();
    }
    std::shared_ptr<Type> copy() const
    {
      return std::make_shared<PointerType>(std::make_shared<PointerType>(ref_->copy()));
    }
    std::string dump()
    {
      return "Pointer(" + ref_->dump() + ")";
    }

  protected:
    std::shared_ptr<Type> ref_;
  };
  std::shared_ptr<PointerType> MakePointerType(std::shared_ptr<Type> ref);

  std::shared_ptr<Type> MakeType(std::shared_ptr<Type> ptr);
}
