#include "VisitorImpl.h"

namespace front
{

    std::any VisitorImpl::visitConstDecl(SysYParser::ConstDeclContext *context)
    {
        try
        {
            context->bType()->accept(this);
            auto allocates = std::vector<std::shared_ptr<ir::Allocate>>();
            for (auto constDef : context->constDef())
            {
                auto allocate = std::any_cast<std::shared_ptr<ir::Allocate>>(constDef->accept(this));
                allocates.push_back(allocate);
            }
            return allocates;
        }
        catch (std::exception &e)
        {
            throw std::runtime_error("Error in visitConstDecl:\n" + std::string(e.what()));
        }
    };

    std::any VisitorImpl::visitConstDef(SysYParser::ConstDefContext *context)
    {
        try
        {
            std::shared_ptr<ir::Allocate> allocate;
            std::string name = context->Identifier()->getSymbol()->getText();
            std::shared_ptr<ir::Type> a_type = ir::MakePrimitiveDataType(ctx_.currentDeclTypeId);

            if (context->constExp().size() > 0)
            {
                auto constExps = context->constExp();
                std::reverse(constExps.begin(), constExps.end());
                for (auto constExp : constExps)
                {
                    auto size_sv = std::any_cast<std::shared_ptr<ir::StaticValue>>(constExp->accept(this));
                    size_t size_val = size_sv->getInt();
                    a_type = MakeArrayType(a_type, size_val);
                }
            }

            std::shared_ptr<ir::StaticValue> initVal = ir::MakeEmptyStaticValue(a_type, name);
            if (context->constInitVal())
            {
                ctx_.currentInitValue.push(initVal);
                context->constInitVal()->accept(this);
                ctx_.currentInitValue.pop();
            }
            else
            {
                throw std::runtime_error("constDef must have initVal");
            }

            allocate = std::make_shared<ir::Allocate>(name, true, initVal);
            allocate->setGlobal(ctx_.symbolTable->getCurrentScope()->isGlobal());
            auto sym = std::make_shared<Symbol>(allocate->getName(), allocate);
            ctx_.symbolTable->addSymbolToCurrentScope(allocate->getName(), sym);

            return allocate;
        }
        catch (std::exception &e)
        {
            throw std::runtime_error("Error in visitConstDef:\n" + std::string(e.what()));
        }
    };

    std::any VisitorImpl::visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *context)
    {
        try
        {
            auto init_target = ctx_.currentInitValue.top();
            auto static_value = std::any_cast<std::shared_ptr<ir::StaticValue>>(context->constExp()->accept(this));
            if (init_target->getType()->isPrimitive() && static_value->getType()->isPrimitive())
            {
                if (std::dynamic_pointer_cast<ir::PrimitiveDataType>(init_target->getType())->getTypeId() == std::dynamic_pointer_cast<ir::PrimitiveDataType>(static_value->getType())->getTypeId())
                {
                    if (init_target->isInt())
                    {
                        init_target->setStaticValue(static_value->getInt());
                    }
                    else if (init_target->isFloat())
                    {
                        init_target->setStaticValue(static_value->getFloat());
                    }
                    else if (init_target->isBool())
                    {
                        init_target->setStaticValue(static_value->getBool());
                    }
                }
            }
            else if (init_target->isArray() && static_value->getType()->isPrimitive())
            {
                auto m_target = init_target;
                while (m_target->isArray())
                {
                    try
                    {
                        m_target = m_target->at(0);
                    }
                    catch (std::exception &e)
                    {
                        throw std::runtime_error("Empty target init array");
                    }
                }
                if (m_target->isInt())
                {
                    m_target->setStaticValue(static_value->getInt());
                }
                else if (m_target->isFloat())
                {
                    m_target->setStaticValue(static_value->getFloat());
                }
                else if (m_target->isBool())
                {
                    m_target->setStaticValue(static_value->getBool());
                }
            }
            else
            {
                throw std::runtime_error("ConstDef for Scalar cannot be init.");
            }
            return 0;
        }
        catch (std::exception &e)
        {
            throw std::runtime_error("Error in visitScalarConstInitVal:\n" + std::string(e.what()));
        }
    };

    std::any VisitorImpl::visitListConstInitVal(SysYParser::ListConstInitValContext *context)
    {
        try
        {
            auto init_target = ctx_.currentInitValue.top();
            if (init_target->isArray())
            {
                int i = 0;
                for (auto initVal : context->constInitVal())
                {
                    ctx_.currentInitValue.push(init_target->at(i));
                    initVal->accept(this);
                    ctx_.currentInitValue.pop();
                    i++;
                }
            }
            else
            {
                throw std::runtime_error("List init must for an array value.");
            }
            return 0;
        }
        catch (std::exception &e)
        {
            throw std::runtime_error("Error in visitListConstInitVal:\n" + std::string(e.what()));
        }
    };
}