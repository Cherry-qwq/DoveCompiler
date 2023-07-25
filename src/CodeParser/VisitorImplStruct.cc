#include <vector>

#include "VisitorImpl.h"

namespace front{
    std::any VisitorImpl::visitFuncDef(SysYParser::FuncDefContext *context)
  {
    auto typeID = std::any_cast<ir::PrimitiveDataType::TypeID>(context->funcType()->accept(this));
    std::string funcName = context->Identifier()->getSymbol()->getText();
    ctx_.currentFunction = std::make_shared<ir::Function>(ir::MakePrimitiveDataType(typeID), funcName);
    ctx_.symbolTable->pushScope("function");

    if (context->block())
    {
      // Add entry basic block to function
      auto entry_bb_name = "_" + funcName + "_entry";
      auto bb = std::make_shared<ir::BasicBlock>(entry_bb_name, ir::BasicBlock::BlockType::FunctionEntry);
      ctx_.currentBasicBlock = bb;
      ctx_.currentFunction->addBasicBlock(bb);

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
  {
    // This block is different from the BasicBlock in ControlFlow, this block is a scope
    ctx_.symbolTable->pushScope("block");
    for (auto blockItem : context->blockItem())
    {
      blockItem->accept(this);
    }
    return 0;
  };
  std::any VisitorImpl::visitBlockItem(SysYParser::BlockItemContext *context)
  {
    if (context->decl())
    {
      context->decl()->accept(this);
      // TODO Add decl allocates to current basic block
    }
    else if (context->stmt())
    {
      context->stmt()->accept(this);
    }
    return 0;
  };
  std::any VisitorImpl::visitAssignmentStmt(SysYParser::AssignmentStmtContext *context)
  { // TODO Add this instructions to current basic block
    return 0;
  };
  std::any VisitorImpl::visitExpStmt(SysYParser::ExpStmtContext *context)
  { // TODO Add this instruction to current basic block
    return 0;
  };
  std::any VisitorImpl::visitBlockStmt(SysYParser::BlockStmtContext *context)
  {
    // Do nothing, just accept.
    context->block()->accept(this);
    return 0;
  };
  std::any VisitorImpl::visitIfStmt(SysYParser::IfStmtContext *context)
  {
    ctx_.symbolTable->pushScope("if");
    auto true_bb_name= "_" + ctx_.currentFunction->getName() + "_if_" + std::to_string(ctx_.basicBlockCounter.next());
    auto exit_bb_name = "_" + ctx_.currentFunction->getName() + "_if_exit_" + std::to_string(ctx_.basicBlockCounter.next());
    auto bb = std::make_shared<ir::BasicBlock>(true_bb_name, ir::BasicBlock::BlockType::IfTrue);
    ctx_.currentFunction->addBasicBlock(bb);
    ctx_.entryBasicBlock = ctx_.currentBasicBlock;
    ctx_.currentBasicBlock = bb;
    auto exit_bb = std::make_shared<ir::BasicBlock>(exit_bb_name, ir::BasicBlock::BlockType::Default);
    ctx_.exitBasicBlock = exit_bb;

    context->stmt()->accept(this);

    ctx_.symbolTable->popScope();
    ctx_.entryBasicBlock = ctx_.currentBasicBlock;
    ctx_.currentBasicBlock = exit_bb;
    ctx_.currentFunction->addBasicBlock(exit_bb);
    return 0;
  };
  std::any VisitorImpl::visitIfElseStmt(SysYParser::IfElseStmtContext *context)
  {
    ctx_.symbolTable->pushScope("if");
    auto true_bb_name = "_" + ctx_.currentFunction->getName() + "_if_" + std::to_string(ctx_.basicBlockCounter.next());
    auto false_bb_name = "_" + ctx_.currentFunction->getName() + "_if_" + std::to_string(ctx_.basicBlockCounter.next());
    auto exit_bb_name = "_" + ctx_.currentFunction->getName() + "_if_exit_" + std::to_string(ctx_.basicBlockCounter.next());
    auto bbt = std::make_shared<ir::BasicBlock>(true_bb_name, ir::BasicBlock::BlockType::IfTrue);
    ctx_.currentFunction->addBasicBlock(bbt);
    ctx_.entryBasicBlock = ctx_.currentBasicBlock;
    ctx_.currentBasicBlock = bbt;
    auto exit_bb = std::make_shared<ir::BasicBlock>(exit_bb_name, ir::BasicBlock::BlockType::Default);

    context->stmt(0)->accept(this);

    ctx_.symbolTable->popScope();

    ctx_.symbolTable->pushScope("else");
    int bbfCount = ctx_.basicBlockCounter.next();
    auto bbf = std::make_shared<ir::BasicBlock>(false_bb_name, ir::BasicBlock::BlockType::IfFalse);
    ctx_.currentFunction->addBasicBlock(bbf);
    ctx_.currentBasicBlock = bbf;

    context->stmt(1)->accept(this);

    ctx_.symbolTable->popScope();
    ctx_.entryBasicBlock = ctx_.currentBasicBlock;
    ctx_.currentBasicBlock = exit_bb;
    ctx_.currentFunction->addBasicBlock(exit_bb);
    return 0;
  };
  std::any VisitorImpl::visitWhileStmt(SysYParser::WhileStmtContext *context)
  {
    ctx_.symbolTable->pushScope("while");
    auto entry_bb_name = "_" + ctx_.currentFunction->getName() + "_while_" + std::to_string(ctx_.basicBlockCounter.next());
    auto exit_bb_name = "_" + ctx_.currentFunction->getName() + "_while_exit_" + std::to_string(ctx_.basicBlockCounter.next());
    auto bb = std::make_shared<ir::BasicBlock>(entry_bb_name, ir::BasicBlock::BlockType::WhileEntry);
    ctx_.currentFunction->addBasicBlock(bb);
    ctx_.entryBasicBlock = ctx_.currentBasicBlock;
    ctx_.currentBasicBlock = bb;
    auto exit_bb = std::make_shared<ir::BasicBlock>(exit_bb_name, ir::BasicBlock::BlockType::Default);
    ctx_.exitBasicBlock = exit_bb;

    context->stmt()->accept(this);

    ctx_.symbolTable->popScope();
    ctx_.entryBasicBlock = ctx_.currentBasicBlock;
    ctx_.currentBasicBlock = exit_bb;
    ctx_.currentFunction->addBasicBlock(exit_bb);
    return 0;
  };
  std::any VisitorImpl::visitBreakStmt(SysYParser::BreakStmtContext *context)
  { // TODO Jump to exit basic block
    return 0;
  };
  std::any VisitorImpl::visitContinueStmt(SysYParser::ContinueStmtContext *context)
  { // TODO Jump to loop entry basic block
    return 0;
  };
  std::any VisitorImpl::visitReturnStmt(SysYParser::ReturnStmtContext *context)
  { // TODO Just add this instruction to current basic block
    return 0;
  };


}