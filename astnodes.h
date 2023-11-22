#pragma once
#include "lexer.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <utility>

//===----------------------------------------------------------------------===//
// AST nodes
//===----------------------------------------------------------------------===//
// 
/// ASTnode - Base class for all AST nodes.
class ASTnode {
public:
  virtual ~ASTnode() {};
  virtual llvm::Value *codegen() = 0;
  virtual std::string to_string() const {return "";};
};

class ExternASTnode : public ASTnode{
public:
  virtual llvm::Value *codegen() override {return nullptr;};
};

class DeclASTnode : public ASTnode{
public:
  virtual llvm::Value *codegen() override {return nullptr;};
};

class ExternListASTnode : public ASTnode{
public:
  std::vector<std::unique_ptr<ExternASTnode>> externs;
  virtual llvm::Value *codegen() override {return nullptr;};
  // ExternListASTnode(){};
};

class DeclListASTnode : public ASTnode{
public:
  std::vector<std::unique_ptr<DeclASTnode>> decls;
  virtual llvm::Value *codegen() override {return nullptr;};
};


class ProgramASTnode : public ASTnode{
public:
  // std::vector<std::unique_ptr<ExternASTnode>> externs;
  // std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::unique_ptr<ExternListASTnode> externlist;
  std::unique_ptr<DeclListASTnode> decllist;
  std::string s = "hi";
  ProgramASTnode(std::unique_ptr<ExternListASTnode> el, 
                 std::unique_ptr<DeclListASTnode> dl) : externlist(std::move(el)), decllist(std::move(dl)){}

  virtual llvm::Value *codegen() override {return nullptr;}
};

class VarDeclASTnode : public DeclASTnode{};
class FuncDeclASTnode : public DeclASTnode{};
class ParamASTnode : public ASTnode{};
// class ArgASTnode : public ASTnode{};
class BlockASTnode : public ASTnode{};
class VarAssignASTnode : public ASTnode{};
class ExprASTnode : public ASTnode{};
class BinOpASTnode : public ASTnode{};
class UnOpASTnode : public ASTnode{};
class LitASTnode : public ASTnode{};
class IfThenElseASTnode : public ASTnode{};
class WhileASTnode : public ASTnode{};
class IntASTnode : public ASTnode {
public:
  /// IntASTnode - Class for integer literals like 1, 2, 10,
  int Val;
  TOKEN Tok;
  std::string Name;
  IntASTnode(TOKEN tok, int val) : Tok(tok), Val(val){}
  virtual llvm::Value *codegen() override{return nullptr;};
  // virtual std::string to_string() const override {
  //   //return a sting representation of this AST node
  // };
};
class BoolASTnode : public ASTnode{};
class FloatASTnode : public ASTnode{};
class IdentASTnode : public ASTnode{};
class FuncCallASTnode : public ASTnode{};


/* add other AST nodes as nessasary */

