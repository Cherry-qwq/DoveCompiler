#include <vector>
#include "VisitorImpl.h"
namespace front
{

  std::any VisitorImpl::visitCompUnit(SysYParser::CompUnitContext *context)
  {
    ctx_.symbolTable->pushScope("global");
    ctx_.compUnit = std::make_shared<ir::CompUnit>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), "compUnit");
    // Every decl is several global variables or constants
    for (auto decl : context->decl())
    {
      auto users = std::any_cast<std::vector<std::shared_ptr<ir::User>>>(decl->accept(this));
      // Add global variables and constants to symbol table
      for (auto user : users)
      {
        std::shared_ptr<Symbol> sym;
        if (user->isAllocate())
          sym = std::make_shared<Symbol>(user->getName(), std::dynamic_pointer_cast<ir::Allocate>(user));
        else if (user->isConstant())
          sym = std::make_shared<Symbol>(user->getName(), std::dynamic_pointer_cast<ir::Constant>(user));
        ctx_.symbolTable->addSymbolToGlobalScope(user->getName(), sym);
        ctx_.compUnit->addGlobalObject(user);
      }
    }
    for (auto funcDef : context->funcDef())
    {
      funcDef->accept(this);
      ctx_.compUnit->addGlobalObject(ctx_.currentFunction);
    }
    return ctx_.compUnit;
  };
  std::any VisitorImpl::visitDecl(SysYParser::DeclContext *context)
  {
    std::vector<std::shared_ptr<ir::User>> users;
    if (context->constDecl())
    {
      // Cast vector<Constant> to vector<User>
      auto consts = std::any_cast<std::vector<std::shared_ptr<ir::Constant>>>(context->constDecl()->accept(this));
      users = std::vector<std::shared_ptr<ir::User>>(consts.begin(), consts.end());
    }
    else if (context->varDecl())
    {
      // Cast vector<Allocate> to vector<User>
      auto vars = std::any_cast<std::vector<std::shared_ptr<ir::Allocate>>>(context->varDecl()->accept(this));
      users = std::vector<std::shared_ptr<ir::User>>(vars.begin(), vars.end());
    }
    return users;
  };
  std::any VisitorImpl::visitConstDecl(SysYParser::ConstDeclContext *context)
  {
    auto typeID = std::any_cast<ir::PrimitiveDataType::TypeID>(context->bType()->accept(this));
    auto constants = std::vector<std::shared_ptr<ir::Constant>>();
    for (auto constDef : context->constDef())
    {
      auto a = std::any_cast<std::shared_ptr<ir::Constant>>(constDef->accept(this));
      constants.push_back(a);
    }
    return constants;
  };
  std::any VisitorImpl::visitBType(SysYParser::BTypeContext *context)
  {
    auto typeID = ir::PrimitiveDataType::TypeID::Int32;
    if (context->Int())
    {
      typeID = ir::PrimitiveDataType::TypeID::Int32;
    }
    else if (context->Float())
    {
      typeID = ir::PrimitiveDataType::TypeID::Float32;
    }
    return typeID;
  };
  std::any VisitorImpl::visitConstDef(SysYParser::ConstDefContext *context)
  {
    std::shared_ptr<ir::Constant> constant;
    std::string name = context->Identifier()->getSymbol()->getText();
    std::shared_ptr<ir::Type> arr_type = ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32);
    if (context->constExp().size() > 0)
    {
      for (auto constExp : context->constExp())
      {
        auto constant = std::any_cast<ir::Constant>(constExp->accept(this));
        int32_t val = constant.getIntVal();
        arr_type = MakeArrayType(arr_type, val);
      }
      // TODO ListConstInitVal
    }
    else
    {
      // TODO ScalarConstInitVal
    }
    context->constInitVal()->accept(this);
    return constant;
  };
  std::any VisitorImpl::visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *context)
  {
    return context->constExp()->accept(this);
  };
  std::any VisitorImpl::visitListConstInitVal(SysYParser::ListConstInitValContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitVarDecl(SysYParser::VarDeclContext *context)
  {
    auto typeID = std::any_cast<ir::PrimitiveDataType::TypeID>(context->bType()->accept(this));
    auto allocates = std::vector<std::shared_ptr<ir::Allocate>>();
    for (auto varDef : context->varDef())
    {
      auto allocate = std::any_cast<std::shared_ptr<ir::Allocate>>(varDef->accept(this));
      if (allocate->getType()->isArray())
      {
        std::dynamic_pointer_cast<ir::ArrayType>(allocate->getType())->setInternalType(ir::MakePrimitiveDataType(typeID));
      }
      else
      {
        allocate->setType(ir::MakePrimitiveDataType(typeID));
      }
      allocates.push_back(allocate);
    }
    return allocates;
  };
  std::any VisitorImpl::visitUnInitVarDef(SysYParser::UnInitVarDefContext *context)
  {
    auto varName = context->Identifier()->getSymbol()->getText();
    auto allocate = std::make_shared<ir::Allocate>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), varName);
    std::shared_ptr<ir::Type> arr_type = MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32);

    if (context->constExp().size() > 0)
    {
      // std::reverse(context->constExp().begin(), context->constExp().end());
      for (auto constExp : context->constExp())
      {
        // idx requests a constant
        auto idx = std::any_cast<std::shared_ptr<ir::Constant>>(constExp->accept(this));
        assert(idx->isInt());
        int32_t val = idx->getIntVal();
        arr_type = MakeArrayType(arr_type, val);
      }
    }
    allocate->setType(arr_type);
    return allocate;
  };
  std::any VisitorImpl::visitInitVarDef(SysYParser::InitVarDefContext *context)
  {
    auto allocate = std::make_shared<ir::Allocate>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), context->Identifier()->getSymbol()->getText());
    std::shared_ptr<ir::Type> arr_type = MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32);

    if (context->constExp().size() > 0)
    {
      for (auto constExp : context->constExp())
      {
        // idx requests a constant
        auto idx = std::any_cast<std::shared_ptr<ir::Constant>>(constExp->accept(this));
        assert(idx->isInt());
        int32_t val = idx->getIntVal();
        arr_type = MakeArrayType(arr_type, val);
      }
      // TODO ListInitVal
    }
    else
    {
      // TODO ScalarInitVal
    }
    allocate->setType(arr_type);
    return allocate;
  };
  std::any VisitorImpl::visitScalarInitVal(SysYParser::ScalarInitValContext *context)
  {
    return context->exp()->accept(this);
  };
  std::any VisitorImpl::visitListInitVal(SysYParser::ListInitValContext *context)
  { // TODO
    return 0;
  };

  std::any VisitorImpl::visitTerminal(antlr4::tree::TerminalNode *node)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitChildren(antlr4::tree::ParseTree *node)
  { // TODO
    return 0;
  };
}