#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "Type.h"
#include "User.h"

namespace ir
{
    class Integer : public User
  {
  public:
    explicit Integer(uint32_t val) : User(MakePrimitiveDataType(PrimitiveDataType::TypeID::Int32),1) {}
    std::string dump() const override {
      return std::to_string(val);
    }
    // [[nodiscard]] bool is_const() const override;

  protected:
    uint32_t val = 0;
  };


}
