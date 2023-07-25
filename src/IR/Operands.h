#pragma once

#include <memory>

#include "Type.h"
#include "Instruction.h"

namespace ir
{
  class Constant : public Instruction
  {
  public:
    explicit Constant(std::shared_ptr<Type> type, std::string name, int32_t val) : Instruction(type, std::move(name), 2), is_int_(true), int_val_(val) { is_constant_ = true; }
    explicit Constant(std::shared_ptr<Type> type, std::string name, float val) : Instruction(type, std::move(name), 2), is_float_(true), float_val_(val) { is_constant_ = true; }
    explicit Constant(std::shared_ptr<Type> type, std::string name, bool val) : Instruction(type, std::move(name), 2), is_bool_(true), bool_val_(val) { is_constant_ = true; }
    explicit Constant(std::shared_ptr<Type> type, std::string name, std::vector<Constant> vals) : Instruction(type, std::move(name), 2), is_array_(true), array_vals_(vals) { is_constant_ = true; }
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Constant ";
      if (is_int_)
        output += std::to_string(int_val_);
      else if (is_float_)
        output += std::to_string(float_val_);
      else if (is_bool_)
        output += std::to_string(bool_val_);
      else
        output += "null";
      helper.add(output);
      return output;
    }

    bool isInt() const
    {
      return is_int_;
    }

    bool isFloat() const
    {
      return is_float_;
    }

    bool isBool() const
    {
      return is_bool_;
    }
    bool isArray() const
    {
      return is_array_;
    }

    int32_t getIntVal() const
    {
      return int_val_;
    }
    float getFloatVal() const
    {
      return float_val_;
    }
    bool getBoolVal() const
    {
      return bool_val_;
    }
    ir::Constant arrAt(size_t index) const
    {
      return array_vals_[index];
    }
    std::vector<ir::Constant> getArrayVal() const
    {
      return array_vals_;
    }

  protected:
    bool is_int_ = false;
    int32_t int_val_;
    bool is_float_ = false;
    float float_val_;
    bool is_bool_ = false;
    bool bool_val_;
    bool is_array_ = false;
    std::vector<Constant> array_vals_;
  };

  class Allocate : public Instruction
  {
  public:
    Allocate(std::shared_ptr<Type> type, std::string name) : Instruction(MakePointerType(type->copy()), std::move(name), 1), type_(std::move(type)) { is_allocate_ = true; };
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = (is_const_ ? "Constant " : "Allocate ") + getName() + ": " + getType()->dump();
      helper.add(output);
      return output;
    }

    void setConst(bool is_const)
    {
      is_const_ = is_const;
    }
    bool getConst() const
    {
      return is_const_;
    }

  protected:
    std::shared_ptr<Type> type_;
    bool is_const_ = false;
  };

}