#include "SymbolTable.h"

namespace front
{
    void SymbolTable::addSymbolToCurrentScope(const std::string &name, const std::shared_ptr<ir::Allocate> &symbol)
    {
      scopes_.top()->addSymbol(name, symbol);
    };

    void SymbolTable::addSymbolToGlobalScope(const std::string &name, const std::shared_ptr<ir::Allocate> &symbol)
    {
      auto now_scope = scopes_.top();
      while (!now_scope->isGlobal())
      {
        now_scope = now_scope->getParent();
      }
      now_scope->addSymbol(name, symbol);
    }

    bool SymbolTable::deleteSymbol(const std::string &name, bool recursive = true)
    {
      auto now_scope = scopes_.top();
      while (!now_scope->deleteSymbol(name))
      {
        if (now_scope->isGlobal())
          return false;

        if (!recursive)
          return false;
        now_scope = now_scope->getParent();
      }
      return true;
    }

    std::optional<std::reference_wrapper<std::shared_ptr<ir::Allocate>>> SymbolTable::getSymbol(const std::string &name, bool recursive = true)
    {
      auto now_scope = scopes_.top();
      while (true)
      {
        auto symbol = now_scope->getSymbol(name);
        if (symbol != std::nullopt)
          return symbol;
        else if (now_scope->isGlobal())
          return std::nullopt;

        if (!recursive)
          return std::nullopt;
        now_scope = now_scope->getParent();
      }
    };

    void SymbolTable::pushScope(const std::string &name = std::string("anonymous"))
    {
      auto scope = std::make_shared<Scope>(name,get());
      if (scopes_.empty())
      {
        scopes_.push(scope);
        scope->setGlobal();
      }
      else
      {
        scope->setParent(scopes_.top());
        scopes_.push(scope);
      }
    }

    void SymbolTable::popScope()
    {
      scopes_.pop();
    }

    std::shared_ptr<Scope> SymbolTable::getCurrentScope()
    {
      return scopes_.top();
    }
}