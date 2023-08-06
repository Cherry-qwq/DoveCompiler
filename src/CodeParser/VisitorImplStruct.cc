#include <vector>

#include "VisitorImpl.h"

namespace front
{

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
        }
        else if (context->stmt())
        {
            context->stmt()->accept(this);
        }
        return 0;
    };

    std::any VisitorImpl::visitAssignmentStmt(SysYParser::AssignmentStmtContext *context)
    {
        try
        {
            auto left = std::any_cast<std::shared_ptr<ir::User>>(context->lVal()->accept(this));
            auto right = std::any_cast<std::shared_ptr<ir::User>>(context->exp()->accept(this));
            auto store = std::make_shared<ir::Store>(right, left);
            ctx_.currentBasicBlock->addInstruction(store);
            return std::dynamic_pointer_cast<ir::User>(store);
        }
        catch (std::exception &e)
        {
            throw std::runtime_error("Error in visitAssignmentStmt:\n" + std::string(e.what()));
        }
    };

    std::any VisitorImpl::visitExpStmt(SysYParser::ExpStmtContext *context)
    {
        try
        {
            return std::any_cast<std::shared_ptr<ir::User>>(context->exp()->accept(this));
        }
        catch (std::exception &e)
        {
            throw std::runtime_error("Error in visitExpStmt:\n" + std::string(e.what()));
        }
    };

    std::any VisitorImpl::visitBlockStmt(SysYParser::BlockStmtContext *context)
    {
        // Do nothing, just accept.
        context->block()->accept(this);
        return 0;
    };

    std::any VisitorImpl::visitCond(SysYParser::CondContext *context)
    {
        return context->lOrExp()->accept(this);
    };

    std::any VisitorImpl::visitIfStmt(SysYParser::IfStmtContext *context)
    {

        auto t_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_if_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto exit_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_if_exit_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto bb = std::make_shared<ir::BasicBlock>(t_jplabel, ir::BasicBlock::BlockType::IfTrue);
        auto exit_bb = std::make_shared<ir::BasicBlock>(exit_jplabel, ir::BasicBlock::BlockType::Default);

        ctx_.symbolTable->pushScope("if");
        ctx_.currentFunction->addBasicBlock(bb);
        ctx_.entryBasicBlock = ctx_.currentBasicBlock;
        ctx_.currentBasicBlock = bb;
        ctx_.exitBasicBlock = exit_bb;
        context->stmt()->accept(this);
        ctx_.symbolTable->popScope();

        ctx_.currentFunction->addBasicBlock(exit_bb);
        ctx_.entryBasicBlock = ctx_.currentBasicBlock;
        ctx_.currentBasicBlock = exit_bb;

        return 0;
    };
    std::any VisitorImpl::visitIfElseStmt(SysYParser::IfElseStmtContext *context)
    {

        auto t_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_if_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto f_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_else_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto exit_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_if_exit_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto bbt = std::make_shared<ir::BasicBlock>(t_jplabel, ir::BasicBlock::BlockType::IfTrue);
        auto bbf = std::make_shared<ir::BasicBlock>(f_jplabel, ir::BasicBlock::BlockType::IfFalse);
        auto exit_bb = std::make_shared<ir::BasicBlock>(exit_jplabel, ir::BasicBlock::BlockType::Default);

        ctx_.symbolTable->pushScope("if");
        ctx_.currentFunction->addBasicBlock(bbt);
        ctx_.entryBasicBlock = ctx_.currentBasicBlock;
        ctx_.currentBasicBlock = bbt;
        context->stmt(0)->accept(this);
        ctx_.symbolTable->popScope();

        ctx_.symbolTable->pushScope("else");
        ctx_.currentFunction->addBasicBlock(bbf);
        ctx_.currentBasicBlock = bbf;
        context->stmt(1)->accept(this);
        ctx_.symbolTable->popScope();

        ctx_.currentFunction->addBasicBlock(exit_bb);
        ctx_.entryBasicBlock = ctx_.currentBasicBlock;
        ctx_.currentBasicBlock = exit_bb;

        return 0;
    };
    std::any VisitorImpl::visitWhileStmt(SysYParser::WhileStmtContext *context)
    {
        auto entry_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_while_entry_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto body_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_while_body_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto exit_jplabel = std::make_shared<ir::JPLabel>("_" + ctx_.currentFunction->getName() + "_while_exit_" + std::to_string(ctx_.basicBlockCounter.next()));
        auto entry_bb = std::make_shared<ir::BasicBlock>(entry_jplabel, ir::BlockType::WhileEntry);
        auto body_bb = std::make_shared<ir::BasicBlock>(body_jplabel, ir::BlockType::WhileBody);
        auto exit_bb = std::make_shared<ir::BasicBlock>(exit_jplabel, ir::BlockType::Default);
        auto entry_label = std::make_shared<ir::Label>(entry_jplabel);
        auto body_label = std::make_shared<ir::Label>(body_jplabel);
        auto exit_label = std::make_shared<ir::Label>(exit_jplabel);
        auto out_exit_bb = ctx_.exitBasicBlock;


        return 0;
    };

    std::any VisitorImpl::visitBreakStmt(SysYParser::BreakStmtContext *context)
    {
        auto label = std::make_shared<ir::Label>(ctx_.breakBBStack.top()->getJPLabel());
        auto br = std::make_shared<ir::Br>(label, label->getName());
        ctx_.currentBasicBlock->addInstruction(br);
        return std::dynamic_pointer_cast<ir::User>(br);
    };

    std::any VisitorImpl::visitContinueStmt(SysYParser::ContinueStmtContext *context)
    {
        auto label = std::make_shared<ir::Label>(ctx_.continueBBStack.top()->getJPLabel());
        auto br = std::make_shared<ir::Br>(label, label->getName());
        ctx_.currentBasicBlock->addInstruction(br);
        return std::dynamic_pointer_cast<ir::User>(br);
    };

    std::any VisitorImpl::visitReturnStmt(SysYParser::ReturnStmtContext *context)
    {
        auto user = std::any_cast<std::shared_ptr<ir::User>>(context->exp()->accept(this));
        auto ret = std::make_shared<ir::Return>(user, user->getName());
        ctx_.currentBasicBlock->addInstruction(ret);
        return std::dynamic_pointer_cast<ir::User>(ret);
    };

    std::any VisitorImpl::visitRelToAddExp(SysYParser::RelToAddExpContext *context)
    {
        try
        {
            auto user = std::any_cast<std::shared_ptr<ir::User>>(context->addExp()->accept(this));
            if (user->getType()->isPrimitive())
            {
                auto type = std::dynamic_pointer_cast<ir::PrimitiveDataType>(user->getType());
                if (type->isInt())
                {
                    auto icmp = std::make_shared<ir::Icmp>(ir::Icmp::IcmpId::NE, user, std::make_shared<ir::StaticValue>("cmp_const", 0), user->getName() + "_icmp");
                    ctx_.currentBasicBlock->addInstruction(icmp);
                    return std::dynamic_pointer_cast<ir::User>(icmp);
                }
                else if (type->isFloat())
                {
                    auto fcmp = std::make_shared<ir::Fcmp>(ir::Fcmp::FcmpId::NE, user, std::make_shared<ir::StaticValue>("cmp_const", 0.0), user->getName() + "_fcmp");
                    ctx_.currentBasicBlock->addInstruction(fcmp);
                    return std::dynamic_pointer_cast<ir::User>(fcmp);
                }
            }
            else
            {
                throw std::runtime_error("Rel must accept a Primitive Value");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitRelToAddExp:\n" + std::string(e.what()));
        }
    };

    std::any VisitorImpl::visitRelTwoExp(SysYParser::RelTwoExpContext *context)
    {
        try
        {
            auto icmpid = ir::Icmp::IcmpId::LT;
            auto fcmpid = ir::Fcmp::FcmpId::LT;
            if (context->LT())
            {
                icmpid = ir::Icmp::IcmpId::LT;
                fcmpid = ir::Fcmp::FcmpId::LT;
            }
            else if (context->LE())
            {
                icmpid = ir::Icmp::IcmpId::LE;
                fcmpid = ir::Fcmp::FcmpId::LE;
            }
            else if (context->GT())
            {
                icmpid = ir::Icmp::IcmpId::GT;
                fcmpid = ir::Fcmp::FcmpId::GT;
            }
            else if (context->GE())
            {
                icmpid = ir::Icmp::IcmpId::GE;
                fcmpid = ir::Fcmp::FcmpId::GE;
            }

            auto left = std::any_cast<std::shared_ptr<ir::User>>(context->relExp()->accept(this));
            auto right = std::any_cast<std::shared_ptr<ir::User>>(context->addExp()->accept(this));
            if (left->isStaticValue() && right->isStaticValue())
            {
                if (left->getType()->isPrimitive() && right->getType()->isPrimitive())
                {
                    auto left_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(left->getType());
                    auto right_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(right->getType());
                    if (left_prim->isInt() && right_prim->isInt())
                    {
                        auto left_const = std::dynamic_pointer_cast<ir::StaticValue>(left);
                        auto right_const = std::dynamic_pointer_cast<ir::StaticValue>(right);
                        auto left_val = left_const->getInt();
                        auto right_val = right_const->getInt();
                        switch (icmpid)
                        {
                        case ir::Icmp::IcmpId::LT:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val < right_val);
                        case ir::Icmp::IcmpId::LE:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val <= right_val);
                        case ir::Icmp::IcmpId::GT:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val > right_val);
                        case ir::Icmp::IcmpId::GE:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val >= right_val);
                        default:
                            throw std::runtime_error("Error in visitRelTwoExp: icmpid not found");
                        }
                    }
                    else if (left_prim->isFloat() && right_prim->isFloat())
                    {
                        auto left_const = std::dynamic_pointer_cast<ir::StaticValue>(left);
                        auto right_const = std::dynamic_pointer_cast<ir::StaticValue>(right);
                        auto left_val = left_const->getFloat();
                        auto right_val = right_const->getFloat();
                        switch (fcmpid)
                        {
                        case ir::Fcmp::FcmpId::LT:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val < right_val);
                        case ir::Fcmp::FcmpId::LE:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val <= right_val);
                        case ir::Fcmp::FcmpId::GT:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val > right_val);
                        case ir::Fcmp::FcmpId::GE:
                            return std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", left_val >= right_val);
                        default:
                            throw std::runtime_error("Error in visitRelTwoExp: fcmpid not found");
                        }
                    }
                }
                throw std::runtime_error("Error in visitRelTwoExp: left and right are not both int or float");
            }
            else
            {
                if (left->getType()->isPrimitive() && right->getType()->isPrimitive())
                {
                    auto left_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(left->getType());
                    auto right_prim = std::dynamic_pointer_cast<ir::PrimitiveDataType>(right->getType());
                    if (left_prim->isInt() && right_prim->isInt())
                    {
                        auto icmp = std::make_shared<ir::Icmp>(icmpid, left, right, "slt");
                        ctx_.currentBasicBlock->addInstruction(icmp);
                        return std::dynamic_pointer_cast<ir::User>(icmp);
                    }
                    else if (left_prim->isFloat() && right_prim->isFloat())
                    {
                        auto fcmp = std::make_shared<ir::Fcmp>(fcmpid, left, right, "flt");
                        ctx_.currentBasicBlock->addInstruction(fcmp);
                        return std::dynamic_pointer_cast<ir::User>(fcmp);
                    }
                }
                throw std::runtime_error("Error in visitRelTwoExp: left and right are not both int or float");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitRelTwoExp:\n" + std::string(e.what()));
        }
    };

    std::any VisitorImpl::visitEqTwoExp(SysYParser::EqTwoExpContext *context)
    {
        try
        {
            
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitEqTwoExp:\n" + std::string(e.what()));
        }
    };
    std::any VisitorImpl::visitEqToRelExp(SysYParser::EqToRelExpContext *context)
    {
        try
        {
            return context->relExp()->accept(this);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitEqToRelExp:\n" + std::string(e.what()));
        }
    };
    std::any VisitorImpl::visitLAndToEqExp(SysYParser::LAndToEqExpContext *context)
    {
        try
        {
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitLAndToEqExp:\n" + std::string(e.what()));
        }
    };
    std::any VisitorImpl::visitLAndTwoExp(SysYParser::LAndTwoExpContext *context)
    {
        try
        {
            auto last = std::any_cast<std::shared_ptr<ir::User>>(context->eqExp()->accept(this));
            auto bb = std::make_shared<ir::BasicBlock>(ctx_.currentFunction, ctx_.currentFunction->getName() + "_land");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitLAndTwoExp:\n" + std::string(e.what()));
        }
    };
    std::any VisitorImpl::visitLOrToLandExp(SysYParser::LOrToLandExpContext *context)
    {
        try
        {
            auto val = std::make_shared<ir::StaticValue>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32), "Bool", true);
            return std::dynamic_pointer_cast<ir::User>(val);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitLOrToLandExp:\n" + std::string(e.what()));
        }
    };
    std::any VisitorImpl::visitLOrTwoExp(SysYParser::LOrTwoExpContext *context)
    {
        try
        {
            auto last = std::any_cast<std::shared_ptr<ir::BasicBlock>>(context->lAndExp()->accept(this));
            auto val = std::make_shared<ir::BasicBlock>(ctx_.currentFunction, "_if_lor_");
            return std::dynamic_pointer_cast<ir::BasicBlock>(val);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error in visitLOrTwoExp:\n" + std::string(e.what()));
        }
    };
}