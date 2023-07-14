#pragma once

#include <set>
#include <memory>
#include <algorithm>
#include <string>

#include "Use.h"
#include "Type.h"
#include "DumpHelper.h"

namespace ir
{
  class Value
  {
    std::set<Use *> uses_;
    std::unique_ptr<Type> type_;
    std::string name_;

  public:
    Value(std::unique_ptr<Type> type, std::string name) : type_(std::move(type)), name_(std::move(name)) {}
    virtual std::string dump(DumpHelper &helper) const = 0;
    bool bindUse(Use *use)
    {
      if (uses_.count(use))
        return false;

      uses_.insert(use);
      return true;
    }
    bool unbindUse(Use *use)
    {
      
      if (!uses_.count(use))
        return false;

      uses_.erase(use);
      return true;
    }

    Value & setName(std::string name) { name_ = std::move(name); return *this; }
    std::string getName() const { return name_; }

    std::unique_ptr<Type> getType() const { return type_->copy(); }

    // virtual bool isConstant() const = 0;
  };
}
