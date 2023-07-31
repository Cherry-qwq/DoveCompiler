#include <vector>
#include <iostream>

#include "VisitorImpl.h"
#include "Utils/Utils.h"
#include "IR/Operators.h"
namespace front
{

  std::any VisitorImpl::visitExp(SysYParser::ExpContext *context)
  {
    return context->addExp()->accept(this);
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
  {
    return context->exp()->accept(this);
  };
  std::any VisitorImpl::visitPrimRvalExp(SysYParser::PrimRvalExpContext *context)
  {
    return context->rVal()->accept(this);
  };
  std::any VisitorImpl::visitPrimConstExp(SysYParser::PrimConstExpContext *context)
  {
    return context->number()->accept(this);
  };
  std::any VisitorImpl::visitNumber(SysYParser::NumberContext *context)
  {
    if (context->IntLiteral())
    {
      auto i = utils::ParseInt32(context->getText());
      auto p = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Int", i);
      return std::dynamic_pointer_cast<ir::User>(p);
    }
    else if (context->FloatLiteral())
    {
      auto i = utils::ParseFloat32(context->getText());
      auto p = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32), "Float", i);
      return std::dynamic_pointer_cast<ir::User>(p);
    }
    throw std::runtime_error("Unknown number type");
    return 0;
  };
  std::any VisitorImpl::visitUnaryToPrimExp(SysYParser::UnaryToPrimExpContext *context)
  {
    return context->primExp()->accept(this);
  };
  std::any VisitorImpl::visitUnaryFuncCallExp(SysYParser::UnaryFuncCallExpContext *context)
  { // TODO
    return 0;
  };
  std::any VisitorImpl::visitUnaryOpExp(SysYParser::UnaryOpExpContext *context)
  {
    auto unaryOp = std::any_cast<char>(context->unaryOp()->accept(this));
    auto right = std::any_cast<std::shared_ptr<ir::Instruction>>(context->unaryExp()->accept(this));
    switch (unaryOp)
    {
    case '+':
    {
      if (right->getType()->isPrimitive())
      {
        auto prim_type = std::dynamic_pointer_cast<ir::PrimitiveDataType>(right->getType());
        if (prim_type->isInt())
        {
          auto left = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Int", int32_t(0));
          auto add = std::make_shared<ir::Add>(left, right, "add");
          return add;
        }
        else if (prim_type->isFloat())
        {
          auto left = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32), "Float", float(0));
          auto fadd = std::make_shared<ir::FAdd>(left, right, "fadd");
          return fadd;
        }
      }
      throw std::runtime_error("Add: Invalid operand types");
      break;
    }
    case '-':
    {
      break;
    }
    case '!':
    {
      break;
    }
    default:
    {
      return right;
      break;
    }
    }
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
  {
    char op = '\0';
    if (context->Multiplication())
    {
      op = '*';
    }
    else if (context->Division())
    {
      op = '/';
    }
    else if (context->Modulo())
    {
      op = '%';
    }

    auto left = std::any_cast<std::shared_ptr<ir::User>>(context->mulExp()->accept(this));
    auto right = std::any_cast<std::shared_ptr<ir::User>>(context->unaryExp()->accept(this));
    if (left->isConstant() && right->isConstant())
    {
      auto left_const = std::dynamic_pointer_cast<ir::Constant>(left);
      auto right_const = std::dynamic_pointer_cast<ir::Constant>(right);
      if (left_const->getType()->isPrimitive() && right_const->getType()->isPrimitive())
      {
        auto left_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(left_const->getType());
        auto right_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(right_const->getType());
        if (left_prim->isInt() && right_prim->isInt())
        {
          int32_t value = 0;
          if (op == '*')
          {
            value = left_const->getIntVal() * right_const->getIntVal();
          }
          else if (op == '/')
          {
            value = left_const->getIntVal() / right_const->getIntVal();
          }
          else if (op == '%')
          {
            value = left_const->getIntVal() % right_const->getIntVal();
          };
          auto result = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Int", int32_t(value));
          return std::dynamic_pointer_cast<ir::User>(result);
        }
        else if (left_prim->isFloat() && right_prim->isFloat())
        {
          float value = 0;
          if (op == '*')
          {
            value = left_const->getFloatVal() * right_const->getFloatVal();
          }
          else if (op == '/')
          {
            value = left_const->getFloatVal() / right_const->getFloatVal();
          }
          else if (op == '%')
          {
            throw std::runtime_error("Float is not support modulo");
          };
          auto result = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32), "Float", float(value));
          return std::dynamic_pointer_cast<ir::User>(result);
        }
      }
    }

    throw std::runtime_error("Mul: Invalid operand types");
    return 0;
  };

  std::any VisitorImpl::visitMulToUnaryExp(SysYParser::MulToUnaryExpContext *context)
  {
    return context->unaryExp()->accept(this);
  };
  std::any VisitorImpl::visitAddToMulExp(SysYParser::AddToMulExpContext *context)
  {
    return context->mulExp()->accept(this);
  };
  std::any VisitorImpl::visitAddTwoExp(SysYParser::AddTwoExpContext *context)
  {
    char op = '\0';
    if (context->Addition())
    {
      op = '+';
    }
    else if (context->Minus())
    {
      op = '-';
    }

    auto left = std::any_cast<std::shared_ptr<ir::User>>(context->addExp()->accept(this));
    auto right = std::any_cast<std::shared_ptr<ir::User>>(context->mulExp()->accept(this));
    if (left->isConstant() && right->isConstant())
    {
      if (left->getType()->isPrimitive() && right->getType()->isPrimitive())
      {

        auto left_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(left->getType());
        auto right_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(right->getType());
        // Int32 add Int32
        if (left_prim->isInt() && right_prim->isInt())
        {
          auto left_const = std::dynamic_pointer_cast<ir::Constant>(left);
          auto right_const = std::dynamic_pointer_cast<ir::Constant>(right);
          auto left_val = left_const->getIntVal();
          auto right_val = right_const->getIntVal();
          auto add = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Int", op == '+' ? left_val + right_val : left_val - right_val);
          return std::dynamic_pointer_cast<ir::User>(add);
          // Float32 add Float32
        }
        else if (left_prim->isFloat() && right_prim->isFloat())
        {
          auto left_const = std::dynamic_pointer_cast<ir::Constant>(left);
          auto right_const = std::dynamic_pointer_cast<ir::Constant>(right);
          auto left_val = left_const->getFloatVal();
          auto right_val = right_const->getFloatVal();
          auto add = std::make_shared<ir::Constant>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32), "Float", op == '+' ? left_val + right_val : left_val - right_val);
          return std::dynamic_pointer_cast<ir::User>(add);
        }
        // pointer add Int ?
      }
    }
    else
    {
      if (left->getType()->isPrimitive() && right->getType()->isPrimitive())
      {
        auto left_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(left->getType());
        auto right_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(right->getType());
        if (left_prim->isInt() && right_prim->isInt())
        {
          auto add = std::make_shared<ir::Add>(left, right, "add");
          return add;
        }
        else if (left_prim->isFloat() && right_prim->isFloat())
        {
          auto fadd = std::make_shared<ir::FAdd>(left, right, "fadd");
          return fadd;
        }
        else
        {
          // TODO
        }
      }
    }
    return 0;
  };
  std::any VisitorImpl::visitRelToAddExp(SysYParser::RelToAddExpContext *context)
  {
    return context->addExp()->accept(this);
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
  {
    return context->relExp()->accept(this);
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
    auto constExp = std::any_cast<std::shared_ptr<ir::User>>(context->addExp()->accept(this));
    if (constExp->isConstant())
    {
      return std::dynamic_pointer_cast<ir::Constant>(constExp);
    }
    throw std::runtime_error("constExp is not a constant");
  };
}