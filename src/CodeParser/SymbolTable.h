#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <stack>
#include <optional>

#include "IR/MemoryAccess.h"

namespace front
{
  class SymbolTable;
  class Scope;

  class Symbol
  {
  public:
    Symbol(const Symbol &symbol) = default;
    Symbol(std::string &name, std::shared_ptr<ir::Allocate> symbol)
        : name_(name), allocate_(symbol){};
    Symbol(std::string &name, std::shared_ptr<ir::GlobalAllocate> symbol)
        : name_(name), global_allocate_(symbol)
    {
      is_global_ = true;
    };
    ~Symbol() = default;

    std::string getName() { return name_; };
    std::shared_ptr<ir::Allocate> getSymbolAllocate() { return allocate_; };
    std::shared_ptr<ir::GlobalAllocate> getSymbolGlobalAllocate() { return global_allocate_; };
    void setConstant() { is_constant_ = true; };

  protected:
    std::string name_;
    std::shared_ptr<ir::Allocate> allocate_;
    std::shared_ptr<ir::GlobalAllocate> global_allocate_;
    std::shared_ptr<Scope> scope_;
    bool is_global_ = false;
    bool is_constant_ = false;
  };

  class Scope
  {
  public:
    explicit Scope(const std::string &name, std::shared_ptr<SymbolTable> symbolTable, std::shared_ptr<Scope> parent)
        : name_(name), symbolTable_(symbolTable), parent_(parent){};
    explicit Scope(const std::string &name, std::shared_ptr<SymbolTable> symbolTable)
        : Scope(name, symbolTable, nullptr){};

    std::shared_ptr<SymbolTable> getSymbolTable() { return symbolTable_; };

    std::shared_ptr<Scope> getParent() { return parent_; };
    void setParent(std::shared_ptr<Scope> parent) { parent_ = parent; };

    void setGlobal() { is_global_ = true; };
    bool isGlobal() { return is_global_; };

    void addSymbol(const std::string &name, const std::shared_ptr<Symbol> &symbol)
    {
      symbols_.insert({name, symbol});
    };

    bool deleteSymbol(const std::string &name)
    {
      return symbols_.erase(name);
    };

    std::optional<std::reference_wrapper<std::shared_ptr<Symbol>>> getSymbol(const std::string name)
    {
      auto current = symbols_.find(name);
      if (current != symbols_.end())
        return current->second; // return value
      return std::nullopt;
    };

  protected:
    std::string name_;
    std::shared_ptr<SymbolTable> symbolTable_;
    std::shared_ptr<Scope> parent_;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols_;
    bool is_global_ = false;
  };

  class SymbolTable : public std::enable_shared_from_this<SymbolTable>
  {
  public:
    SymbolTable() = default;
    ~SymbolTable() = default;

    void addSymbolToCurrentScope(const std::string &name, const std::shared_ptr<Symbol> &symbol);

    void addSymbolToGlobalScope(const std::string &name, const std::shared_ptr<Symbol> &symbol);

    bool deleteSymbol(const std::string &name, bool recursive);

    std::optional<std::reference_wrapper<std::shared_ptr<Symbol>>> getSymbol(const std::string &name, bool recursive);

    void pushScope(const std::string &name);

    void popScope();

    std::shared_ptr<Scope> getCurrentScope();

  protected:
    std::stack<std::shared_ptr<Scope>> scopes_;
  };

}