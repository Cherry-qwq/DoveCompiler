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
        ctx_.currentDeclTypeId = typeID;
        return typeID;
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