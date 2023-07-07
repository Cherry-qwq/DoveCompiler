#pragma once
#include <memory>
#include <string>

namespace IR
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

    PrimitiveDataType(TypeID id);

    std::unique_ptr<Type> get() const override;
    std::string dump() override;

    protected:
      TypeID type_;
  };
  std::unique_ptr<PrimitiveDataType> makePrimitiveDataType(PrimitiveDataType::TypeID id);

  class PointerType : public Type
  {
  public:
    explicit PointerType(std::unique_ptr<Type> ref);
    PointerType(const PointerType &other);
    std::unique_ptr<Type> get() const override;
    std::string dump() override;

  protected:
    std::unique_ptr<Type> ref_;
  };

  std::unique_ptr<Type> makeType(std::unique_ptr<Type> ref);
  std::unique_ptr<PointerType> makePointer(std::unique_ptr<Type> ref);
}
