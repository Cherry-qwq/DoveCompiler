#pragma once

#include <memory>
#include <string>

#include "User.h"
#include "GlobalObject.h"
namespace ir
{

  class CompUnit : public User
  {
  public:
    CompUnit(std::unique_ptr<Type> type, std::string name)
        : User(std::move(type), name){};
    std::string dump(DumpHelper &helper) const override
    {
      return "CompUnit " + getName() + " "; //+ getType()->dump();
    }

    void addGlobalObject(std::shared_ptr<GlobalObject> global_object)
    {
      global_objects_.push_back(std::move(global_object));
    }

    std::shared_ptr<std::vector<std::shared_ptr<GlobalObject>>> getGlobalObjects()
    {
      return std::make_shared<std::vector<std::shared_ptr<GlobalObject>>>(global_objects_);
    }

    bool isConstant() const override
    {
      return false;
    }
    
  protected:
    std::vector<std::shared_ptr<GlobalObject>> global_objects_;
    // TODO
  };

  class Function : public GlobalObject
  {
    // TODO
  };

  class Icmp : public User
  {
    // TODO
  };

  class Branch : public User
  {
    // TODO
  };

  class Return : public User
  {
    // TODO
  };

  class Phi : public User
  {
    // TODO
  };
}
