#include "vector"
#include "VisitorImpl.h"
#include "Utils/Utils.h"
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
      if(allocate->getType()->isArray()){
        std::dynamic_pointer_cast<ir::ArrayType>(allocate->getType())->setInternalType(ir::MakePrimitiveDataType(typeID));
      }else{
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

    if (context->constExp().size() > 0){
      // std::reverse(context->constExp().begin(), context->constExp().end());
      for (auto constExp: context->constExp())
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
  std::any VisitorImpl::visitFuncDef(SysYParser::FuncDefContext *context)
  {
    auto typeID = std::any_cast<ir::PrimitiveDataType::TypeID>(context->funcType()->accept(this));
    std::string funcName = context->Identifier()->getSymbol()->getText();
    ctx_.currentFunction = std::make_shared<ir::Function>(ir::MakePrimitiveDataType(typeID), funcName);
    ctx_.symbolTable->pushScope("function");

    if (context->block())
    {
      // Add entry basic block to function
      auto bb = std::make_shared<ir::BasicBlock>("_" + funcName + "_entry");
      ctx_.currentBasicBlock = bb;
      ctx_.currentFunction->addBasicBlock(ctx_.currentBasicBlock);
      context->block()->accept(this);
    }

    ctx_.symbolTable->popScope();
    return ctx_.currentFunction;
  };
  std::any VisitorImpl::visitFuncType(SysYParser::FuncTypeContext *context)
  {
    auto typeID = ir::PrimitiveDataType::TypeID::Void;
    if (context->Void())
    {
      typeID = ir::PrimitiveDataType::TypeID::Void;
    }
    else if (context->Int())
    {
      typeID = ir::PrimitiveDataType::TypeID::Int32;
    }
    else if (context->Float())
    {
      typeID = ir::PrimitiveDataType::TypeID::Float32;
    }
    return typeID;
  };
  std::any VisitorImpl::visitFuncFparamList(SysYParser::FuncFparamListContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitFuncFparam(SysYParser::FuncFparamContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitBlock(SysYParser::BlockContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitBlockItem(SysYParser::BlockItemContext *context)
  {
    // TODO
    return 0;
  };
  std::any VisitorImpl::visitAssignmentStmt(SysYParser::AssignmentStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitExpStmt(SysYParser::ExpStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitBlockStmt(SysYParser::BlockStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitIfStmt(SysYParser::IfStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitIfElseStmt(SysYParser::IfElseStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitWhileStmt(SysYParser::WhileStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitBreakStmt(SysYParser::BreakStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitContinueStmt(SysYParser::ContinueStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitReturnStmt(SysYParser::ReturnStmtContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitExp(SysYParser::ExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitCond(SysYParser::CondContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitLVal(SysYParser::LValContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitRVal(SysYParser::RValContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitPrimParenExp(SysYParser::PrimParenExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitPrimRvalExp(SysYParser::PrimRvalExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitPrimConstExp(SysYParser::PrimConstExpContext *context)
  { // TODO
    if (context->number())
    {
      return context->number()->accept(this);
    }
    return 0;
  };
  std::any VisitorImpl::visitNumber(SysYParser::NumberContext *context)
  { // TODO
    auto p = std::shared_ptr<ir::Value>();
    if (context->IntLiteral())
    {
      auto i = utils::ParseInt32(context->getText());
      p = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Int", i);
      // Constant(std::shared_ptr<Type> type, uint32_t val)
    }
    else if (context->FloatLiteral())
    {
      auto i = utils::ParseFloat32(context->getText());
      p = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32), "Float", i);
    }
    // std::vector<std::shared_ptr<ir::Allocate>> allocates;

    return p;
  };
  std::any VisitorImpl::visitUnaryToPrimExp(SysYParser::UnaryToPrimExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitUnaryFuncCallExp(SysYParser::UnaryFuncCallExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitUnaryOpExp(SysYParser::UnaryOpExpContext *context)
  {
    auto unaryOp = std::any_cast<char>(context->unaryOp()->accept(this));
    return 0;
  };
  std::any VisitorImpl::visitUnaryOp(SysYParser::UnaryOpContext *context)
  {
    if (context->Addition())
    {
      return '+';
    }
    else if (context->Minus())
    {
      return '-';
    }
    else if (context->Exclamation())
    {
      return '!';
    }
    return '\0';
  };
  std::any VisitorImpl::visitFuncRparamList(SysYParser::FuncRparamListContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitExpAsRparam(SysYParser::ExpAsRparamContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitStringAsRparam(SysYParser::StringAsRparamContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitMulTwoExp(SysYParser::MulTwoExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitMulToUnaryExp(SysYParser::MulToUnaryExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitAddToMulExp(SysYParser::AddToMulExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitAddTwoExp(SysYParser::AddTwoExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitRelToAddExp(SysYParser::RelToAddExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitRelTwoExp(SysYParser::RelTwoExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitEqTwoExp(SysYParser::EqTwoExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitEqToRelExp(SysYParser::EqToRelExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitLAndToEqExp(SysYParser::LAndToEqExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitLAndTwoExp(SysYParser::LAndTwoExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitLOrToLandExp(SysYParser::LOrToLandExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitLOrTwoExp(SysYParser::LOrTwoExpContext *context)
  { // TODO
    return 0;
  };
  //* @return ir::Constant
  std::any VisitorImpl::visitConstExp(SysYParser::ConstExpContext *context)
  { 
    auto constant = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Int", int32_t(2));
    return constant;
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