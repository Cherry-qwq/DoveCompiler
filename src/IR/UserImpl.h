#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "Type.h"
#include "User.h"

namespace ir
{
    class Constant : public User
  {
  public:
    explicit Constant(uint32_t val) : User(MakePrimitiveDataType(PrimitiveDataType::TypeID::Int32),"Constant") {}
    std::string dump() const override {
      return std::to_string(val);
    }
    // [[nodiscard]] bool is_const() const override;

  protected:
    uint32_t val = 0;
  };


}
