#include <vector>
#include "VisitorImpl.h"
namespace front
{

    std::any VisitorImpl::visitCompUnit(SysYParser::CompUnitContext *context)
    {
        ctx_.symbolTable->pushScope("global");
        ctx_.compUnit = std::make_shared<ir::CompUnit>(ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), "compUnit");
        // Every decl is several global variables or StaticValues
        for (auto decl : context->decl())
        {
            auto users = std::any_cast<std::vector<std::shared_ptr<ir::User>>>(decl->accept(this));
            // Add global variables and StaticValues to symbol table
            for (auto user : users)
            {
                std::shared_ptr<Symbol> sym;
                if (user->isAllocate())
                {
                    auto allocate = std::dynamic_pointer_cast<ir::Allocate>(user);
                    sym = std::make_shared<Symbol>(user->getName(), std::dynamic_pointer_cast<ir::Allocate>(user));
                    ctx_.symbolTable->addSymbolToGlobalScope(user->getName(), sym);
                    ctx_.compUnit->addGlobalObject(user);
                }
                else
                {
                    throw std::runtime_error("Only memory allocation or function is allowed in global scope.");
                }
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
            // Cast vector<StaticValue> to vector<User>
            auto consts = std::any_cast<std::vector<std::shared_ptr<ir::Allocate>>>(context->constDecl()->accept(this));
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
        auto allocates = std::vector<std::shared_ptr<ir::Allocate>>();
        for (auto constDef : context->constDef())
        {
            auto a = std::any_cast<std::shared_ptr<ir::Allocate>>(constDef->accept(this));
            allocates.push_back(a);
        }
        return allocates;
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
        std::shared_ptr<ir::Allocate> allocate;
        std::string name = context->Identifier()->getSymbol()->getText();
        std::shared_ptr<ir::Type> a_type = ir::MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32);
        if (context->constExp().size() > 0)
        {
            for (auto constExp : context->constExp())
            {
                auto size_sv = std::any_cast<ir::StaticValue>(constExp->accept(this));
                int32_t size_val = size_sv.getIntVal();
                a_type = MakeArrayType(a_type, size_val);
            }
        }
        allocate = std::make_shared<ir::Allocate>(a_type, name);

        if (context->constInitVal())
        {
            auto initVal = std::any_cast<std::shared_ptr<ir::User>>(context->constInitVal()->accept(this));
            if (initVal->isStaticValue())
            {
                allocate->setInitValue(std::dynamic_pointer_cast<ir::StaticValue>(initVal));
            }
        }
        else
        {
            throw std::runtime_error("constDef must have initVal");
        }

        return allocate;
    };

    std::any VisitorImpl::visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *context)
    {
        // constExp requires a StaticValue
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
        std::shared_ptr<ir::Allocate> allocate;
        auto varName = context->Identifier()->getSymbol()->getText();
        std::shared_ptr<ir::Type> arr_type = MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32);

        if (context->constExp().size() > 0)
        {
            // std::reverse(context->constExp().begin(), context->constExp().end());
            for (auto constExp : context->constExp())
            {
                // idx requests a StaticValue
                auto size_sv = std::any_cast<std::shared_ptr<ir::StaticValue>>(constExp->accept(this));
                assert(size_sv->isInt());
                int32_t size_val = size_sv->getIntVal();
                arr_type = MakeArrayType(arr_type, size_val);
            }
        }
        allocate = std::make_shared<ir::Allocate>(arr_type, varName);
        return allocate;
    };

    std::any VisitorImpl::visitInitVarDef(SysYParser::InitVarDefContext *context)
    {
        std::shared_ptr<ir::Allocate> allocate;
        auto varName = context->Identifier()->getSymbol()->getText();
        std::shared_ptr<ir::Type> a_type = MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32);

        if (context->constExp().size() > 0)
        {
            for (auto constExp : context->constExp())
            {
                // idx requests a StaticValue
                auto size_sv = std::any_cast<std::shared_ptr<ir::StaticValue>>(constExp->accept(this));
                assert(size_sv->isInt());
                int32_t size_val = size_sv->getIntVal();
                a_type = MakeArrayType(a_type, size_val);
            }
        }

        allocate = std::make_shared<ir::Allocate>(a_type, varName);

        auto init_val = std::any_cast<std::shared_ptr<ir::User>>(context->initVal()->accept(this));
        if (init_val->isStaticValue())
        {
            // static init
            allocate->setInitValue(std::dynamic_pointer_cast<ir::StaticValue>(init_val));
        }
        else
        {
            // TODO add ir stmts for dynamic init
        }

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