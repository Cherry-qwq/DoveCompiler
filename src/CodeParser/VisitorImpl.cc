#include "vector"
#include "VisitorImpl.h"

#include "Utils/Utils.h"

namespace front
{

  std::any VisitorImpl::visitCompUnit(SysYParser::CompUnitContext *context)
  {
    ctx_.symbolTable->pushScope("global");
    ctx_.compUnit = std::make_shared<ir::CompUnit>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), "compUnit");
    
    for (auto decl : context->decl())
    {
      std::vector<std::shared_ptr<ir::Allocate>> allocates;
      allocates = std::any_cast<std::vector<std::shared_ptr<ir::Allocate>>>(decl->accept(this));
      //allocates=decl->accept(this);
      for(auto allo : allocates){
          std::shared_ptr<Symbol> sym = std::make_shared<Symbol>(allo->getName(), allo);
          ctx_.symbolTable->addSymbolToGlobalScope(allo->getName(), sym);
          ctx_.compUnit->addGlobalObject(allo);
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
    std::vector<std::shared_ptr<ir::Allocate>> allocates;
    if (context->constDecl())
    {
      allocates = std::any_cast<std::vector<std::shared_ptr<ir::Allocate>>>(context->constDecl()->accept(this));
    }
    else if (context->varDecl())
    {
      allocates = std::any_cast<std::vector<std::shared_ptr<ir::Allocate>>>(context->varDecl()->accept(this));
    }
    return allocates;
  };
  std::any VisitorImpl::visitConstDecl(SysYParser::ConstDeclContext *context)
  {
    ir::PrimitiveDataType::TypeID typeID = std::any_cast<ir::PrimitiveDataType::TypeID>(context->bType()->accept(this));
    std::vector<std::shared_ptr<ir::Allocate>> allocates;
    for (auto constDef : context->constDef())
    {
      std::shared_ptr<ir::Allocate> a = std::any_cast<std::shared_ptr<ir::Allocate>>(constDef->accept(this));
      a->setType(ir::MakePrimitiveDataType(typeID));
      allocates.push_back(a);
    }
    return allocates;
  };
  std::any VisitorImpl::visitBType(SysYParser::BTypeContext *context)
  {
    ir::PrimitiveDataType::TypeID typeID = ir::PrimitiveDataType::TypeID::Int32;
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
    std::shared_ptr<ir::Allocate> allocate = std::make_shared<ir::Allocate>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), context->Identifier()->getSymbol()->getText());
    context->constInitVal()->accept(this);
    allocate->setConst(true);
    return allocate;
  };
  std::any VisitorImpl::visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitListConstInitVal(SysYParser::ListConstInitValContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitVarDecl(SysYParser::VarDeclContext *context)
  {
    ir::PrimitiveDataType::TypeID typeID = std::any_cast<ir::PrimitiveDataType::TypeID>(context->bType()->accept(this));
    std::vector<std::shared_ptr<ir::Allocate>> allocates;
    for (auto varDef : context->varDef())
    {
      std::shared_ptr<ir::Allocate> a = std::any_cast<std::shared_ptr<ir::Allocate>>(varDef->accept(this));
      a->setType(ir::MakePrimitiveDataType(typeID));
      allocates.push_back(a);
    }
    return allocates;
  };
  std::any VisitorImpl::visitUnInitVarDef(SysYParser::UnInitVarDefContext *context)
  {
    std::shared_ptr<ir::Allocate> allocate = std::make_shared<ir::Allocate>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), context->Identifier()->getSymbol()->getText());
    return allocate;
  };
  std::any VisitorImpl::visitInitVarDef(SysYParser::InitVarDefContext *context)
  { 
    std::shared_ptr<ir::Allocate> allocate = std::make_shared<ir::Allocate>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), context->Identifier()->getSymbol()->getText());
    context->initVal()->accept(this);
    return allocate;
  };
  std::any VisitorImpl::visitScalarInitVal(SysYParser::ScalarInitValContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitListInitVal(SysYParser::ListInitValContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitFuncDef(SysYParser::FuncDefContext *context)
  {
    ir::PrimitiveDataType::TypeID typeID = std::any_cast<ir::PrimitiveDataType::TypeID>(context->funcType()->accept(this));
    ctx_.currentFunction = std::make_shared<ir::Function>(ir::MakePrimitiveDataType(typeID), context->Identifier()->getSymbol()->getText());
    ctx_.symbolTable->pushScope("function");

    for (auto blockItem : context->block()->blockItem())
    {
      blockItem->accept(this);
    }

    ctx_.symbolTable->popScope();
    return ctx_.currentFunction;
  };
  std::any VisitorImpl::visitFuncType(SysYParser::FuncTypeContext *context)
  {
    ir::PrimitiveDataType::TypeID typeID = ir::PrimitiveDataType::TypeID::Void;
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
  { 
 

    return 0;
  };
  std::any VisitorImpl::visitNumber(SysYParser::NumberContext *context)
  { // TODO
    return 0;
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
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitUnaryOp(SysYParser::UnaryOpContext *context)
  { // TODO
    return 0;
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
  std::any VisitorImpl::visitConstExp(SysYParser::ConstExpContext *context)
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