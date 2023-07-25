#pragma once

#include <cstdint>
#include <variant>
#include <memory>

#include <antlr4-runtime.h>
#include <SysYParser.h>
#include <SysYVisitor.h>

#include "SymbolTable.h"
#include "IR/BasicBlock.h"
#include "IR/Operands.h"
#include "IR/MemoryAccess.h"
#include "IR/ControlFlow.h"
#include "Utils/Counter.h"

namespace front
{

  class Context
  {
  public:
    std::shared_ptr<SymbolTable> symbolTable = std::make_shared<SymbolTable>();

    std::shared_ptr<ir::CompUnit> compUnit;
    std::shared_ptr<ir::Function> currentFunction;

    std::vector<std::shared_ptr<ir::Allocate>> currentAllocates;

    std::shared_ptr<ir::BasicBlock> entryBasicBlock;
    std::shared_ptr<ir::BasicBlock> currentBasicBlock;
    std::shared_ptr<ir::BasicBlock> exitBasicBlock;
    utils::Counter basicBlockCounter = utils::Counter(0);

    std::shared_ptr<ir::Allocate> returnAllocate;
  };

  class VisitorImpl : public SysYVisitor
  {
  public:
    std::any visitCompUnit(SysYParser::CompUnitContext *context) override;
    std::any visitDecl(SysYParser::DeclContext *context) override;
    std::any visitConstDecl(SysYParser::ConstDeclContext *context) override;
    std::any visitBType(SysYParser::BTypeContext *context) override;
    std::any visitConstDef(SysYParser::ConstDefContext *context) override;
    std::any visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *context) override;
    std::any visitListConstInitVal(SysYParser::ListConstInitValContext *context) override;
    std::any visitVarDecl(SysYParser::VarDeclContext *context) override;
    std::any visitUnInitVarDef(SysYParser::UnInitVarDefContext *context) override;
    std::any visitInitVarDef(SysYParser::InitVarDefContext *context) override;
    std::any visitScalarInitVal(SysYParser::ScalarInitValContext *context) override;
    std::any visitListInitVal(SysYParser::ListInitValContext *context) override;
    std::any visitFuncDef(SysYParser::FuncDefContext *context) override;
    std::any visitFuncType(SysYParser::FuncTypeContext *context) override;
    std::any visitFuncFparamList(SysYParser::FuncFparamListContext *context) override;
    std::any visitFuncFparam(SysYParser::FuncFparamContext *context) override;
    std::any visitBlock(SysYParser::BlockContext *context) override;
    std::any visitBlockItem(SysYParser::BlockItemContext *context) override;
    std::any visitAssignmentStmt(SysYParser::AssignmentStmtContext *context) override;
    std::any visitExpStmt(SysYParser::ExpStmtContext *context) override;
    std::any visitBlockStmt(SysYParser::BlockStmtContext *context) override;
    std::any visitIfStmt(SysYParser::IfStmtContext *context) override;
    std::any visitIfElseStmt(SysYParser::IfElseStmtContext *context) override;
    std::any visitWhileStmt(SysYParser::WhileStmtContext *context) override;
    std::any visitBreakStmt(SysYParser::BreakStmtContext *context) override;
    std::any visitContinueStmt(SysYParser::ContinueStmtContext *context) override;
    std::any visitReturnStmt(SysYParser::ReturnStmtContext *context) override;
    std::any visitExp(SysYParser::ExpContext *context) override;
    std::any visitCond(SysYParser::CondContext *context) override;
    std::any visitLVal(SysYParser::LValContext *context) override;
    std::any visitRVal(SysYParser::RValContext *context) override;
    std::any visitPrimParenExp(SysYParser::PrimParenExpContext *context) override;
    std::any visitPrimRvalExp(SysYParser::PrimRvalExpContext *context) override;
    std::any visitPrimConstExp(SysYParser::PrimConstExpContext *context) override;
    std::any visitNumber(SysYParser::NumberContext *context) override;
    std::any visitUnaryToPrimExp(SysYParser::UnaryToPrimExpContext *context) override;
    std::any visitUnaryFuncCallExp(SysYParser::UnaryFuncCallExpContext *context) override;
    std::any visitUnaryOpExp(SysYParser::UnaryOpExpContext *context) override;
    std::any visitUnaryOp(SysYParser::UnaryOpContext *context) override;
    std::any visitFuncRparamList(SysYParser::FuncRparamListContext *context) override;
    std::any visitExpAsRparam(SysYParser::ExpAsRparamContext *context) override;
    std::any visitStringAsRparam(SysYParser::StringAsRparamContext *context) override;
    std::any visitMulTwoExp(SysYParser::MulTwoExpContext *context) override;
    std::any visitMulToUnaryExp(SysYParser::MulToUnaryExpContext *context) override;
    std::any visitAddToMulExp(SysYParser::AddToMulExpContext *context) override;
    std::any visitAddTwoExp(SysYParser::AddTwoExpContext *context) override;
    std::any visitRelToAddExp(SysYParser::RelToAddExpContext *context) override;
    std::any visitRelTwoExp(SysYParser::RelTwoExpContext *context) override;
    std::any visitEqTwoExp(SysYParser::EqTwoExpContext *context) override;
    std::any visitEqToRelExp(SysYParser::EqToRelExpContext *context) override;
    std::any visitLAndToEqExp(SysYParser::LAndToEqExpContext *context) override;
    std::any visitLAndTwoExp(SysYParser::LAndTwoExpContext *context) override;
    std::any visitLOrToLandExp(SysYParser::LOrToLandExpContext *context) override;
    std::any visitLOrTwoExp(SysYParser::LOrTwoExpContext *context) override;
    std::any visitConstExp(SysYParser::ConstExpContext *context) override;

    std::any visitTerminal(antlr4::tree::TerminalNode *node) override;
    std::any visitChildren(antlr4::tree::ParseTree *node) override;

  protected:
    Context ctx_ = Context();
  };
}
