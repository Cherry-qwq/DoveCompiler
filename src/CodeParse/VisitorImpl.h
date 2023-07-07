#pragma once

#include <cstdint>
#include <variant>
#include <antlr4-runtime.h>
#include <SysYParser.h>
#include <SysYVisitor.h>

namespace CodeParser
{

  class VisitorImpl : public SysYVisitor
  {
    std::any visitCompUnit(SysYParser::CompUnitContext *context) = 0;

    std::any visitDecl(SysYParser::DeclContext *context) = 0;

    std::any visitConstDecl(SysYParser::ConstDeclContext *context) = 0;

    std::any visitBType(SysYParser::BTypeContext *context) = 0;

    std::any visitConstDef(SysYParser::ConstDefContext *context) = 0;

    std::any visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *context) = 0;

    std::any visitListConstInitVal(SysYParser::ListConstInitValContext *context) = 0;

    std::any visitVarDecl(SysYParser::VarDeclContext *context) = 0;

    std::any visitUnInitVarDef(SysYParser::UnInitVarDefContext *context) = 0;

    std::any visitInitVarDef(SysYParser::InitVarDefContext *context) = 0;

    std::any visitScalarInitVal(SysYParser::ScalarInitValContext *context) = 0;

    std::any visitListInitVal(SysYParser::ListInitValContext *context) = 0;

    std::any visitFuncDef(SysYParser::FuncDefContext *context) = 0;

    std::any visitFuncType(SysYParser::FuncTypeContext *context) = 0;

    std::any visitFuncFparamList(SysYParser::FuncFparamListContext *context) = 0;

    std::any visitFuncFparam(SysYParser::FuncFparamContext *context) = 0;

    std::any visitBlock(SysYParser::BlockContext *context) = 0;

    std::any visitBlockItem(SysYParser::BlockItemContext *context) = 0;

    std::any visitAssignmentStmt(SysYParser::AssignmentStmtContext *context) = 0;

    std::any visitExpStmt(SysYParser::ExpStmtContext *context) = 0;

    std::any visitBlockStmt(SysYParser::BlockStmtContext *context) = 0;

    std::any visitIfStmt(SysYParser::IfStmtContext *context) = 0;

    std::any visitIfElseStmt(SysYParser::IfElseStmtContext *context) = 0;

    std::any visitWhileStmt(SysYParser::WhileStmtContext *context) = 0;

    std::any visitBreakStmt(SysYParser::BreakStmtContext *context) = 0;

    std::any visitContinueStmt(SysYParser::ContinueStmtContext *context) = 0;

    std::any visitReturnStmt(SysYParser::ReturnStmtContext *context) = 0;

    std::any visitExp(SysYParser::ExpContext *context) = 0;

    std::any visitCond(SysYParser::CondContext *context) = 0;

    std::any visitLVal(SysYParser::LValContext *context) = 0;

    std::any visitRVal(SysYParser::RValContext *context) = 0;

    std::any visitPrimParenExp(SysYParser::PrimParenExpContext *context) = 0;

    std::any visitPrimRvalExp(SysYParser::PrimRvalExpContext *context) = 0;

    std::any visitPrimConstExp(SysYParser::PrimConstExpContext *context) = 0;

    std::any visitNumber(SysYParser::NumberContext *context) = 0;

    std::any visitUnaryToPrimExp(SysYParser::UnaryToPrimExpContext *context) = 0;

    std::any visitUnaryFuncCallExp(SysYParser::UnaryFuncCallExpContext *context) = 0;

    std::any visitUnaryOpExp(SysYParser::UnaryOpExpContext *context) = 0;

    std::any visitUnaryOp(SysYParser::UnaryOpContext *context) = 0;

    std::any visitFuncRparamList(SysYParser::FuncRparamListContext *context) = 0;

    std::any visitExpAsRparam(SysYParser::ExpAsRparamContext *context) = 0;

    std::any visitStringAsRparam(SysYParser::StringAsRparamContext *context) = 0;

    std::any visitMulTwoExp(SysYParser::MulTwoExpContext *context) = 0;

    std::any visitMulToUnaryExp(SysYParser::MulToUnaryExpContext *context) = 0;

    std::any visitAddToMulExp(SysYParser::AddToMulExpContext *context) = 0;

    std::any visitAddTwoExp(SysYParser::AddTwoExpContext *context) = 0;

    std::any visitRelToAddExp(SysYParser::RelToAddExpContext *context) = 0;

    std::any visitRelTwoExp(SysYParser::RelTwoExpContext *context) = 0;

    std::any visitEqTwoExp(SysYParser::EqTwoExpContext *context) = 0;

    std::any visitEqToRelExp(SysYParser::EqToRelExpContext *context) = 0;

    std::any visitLAndToEqExp(SysYParser::LAndToEqExpContext *context) = 0;

    std::any visitLAndTwoExp(SysYParser::LAndTwoExpContext *context) = 0;

    std::any visitLOrToLandExp(SysYParser::LOrToLandExpContext *context) = 0;

    std::any visitLOrTwoExp(SysYParser::LOrTwoExpContext *context) = 0;

    std::any visitConstExp(SysYParser::ConstExpContext *context) = 0;
  };
}
