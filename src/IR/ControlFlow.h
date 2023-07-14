#pragma once

#include <memory>
#include <vector>
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
      std::string output = "CompUnit " + getName() + " ";
      for(auto global_object : global_objects_)
      {
        output += "\n\t" + global_object->dump(helper);
      }
      return output;
    }

    void addGlobalObject(std::shared_ptr<GlobalObject> global_object)
    {
      global_objects_.push_back(std::move(global_object));
    }

    std::shared_ptr<std::vector<std::shared_ptr<GlobalObject>>> getGlobalObjects()
    {
      return std::make_shared<std::vector<std::shared_ptr<GlobalObject>>>(global_objects_);
    }

    // bool isConstant() const override
    // {
    //   return false;
    // }
    
  protected:
    std::vector<std::shared_ptr<GlobalObject>> global_objects_;
  };

  class Function : public GlobalObject
  {
  public:
    Function(std::unique_ptr<Type> type, std::string name)
        : GlobalObject(std::move(type), std::move(name)){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Function " + getName() + " ";
      for(auto basic_block : basic_blocks_)
      {
        output += "\n\t" + basic_block->dump(helper);
      }
      return output;
    }

    void addBasicBlock(std::shared_ptr<BasicBlock> basic_block)
    {
      basic_blocks_.push_back(std::move(basic_block));
    }

    std::shared_ptr<std::vector<std::shared_ptr<BasicBlock>>> getBasicBlocks()
    {
      return std::make_shared<std::vector<std::shared_ptr<BasicBlock>>>(basic_blocks_);
    }

    // bool isConstant() const override
    // {
    //   return false;
    // }

  protected:
    std::vector<std::shared_ptr<BasicBlock>> basic_blocks_;
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
