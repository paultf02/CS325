#pragma once
#include "lexer.h"
#include "llvm/IR/Value.h"

//===----------------------------------------------------------------------===//
// AST nodes
//===----------------------------------------------------------------------===//
// 
/// ASTnode - Base class for all AST nodes.
class ASTnode {
public:
  virtual ~ASTnode() {}
  virtual llvm::Value *codegen() = 0;
  virtual std::string to_string() const {
    return "";
  };
};
class ProgramASTnode : public ASTnode{
public:
  std::vector<std::unique_ptr<ExternASTnode>> externs;
  std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::string s = "hi";
  ProgramASTnode(std::vector<std::unique_ptr<ExternASTnode>> e, 
                 std::vector<std::unique_ptr<DeclASTnode>> d) : externs(e), decls(d){}
  // ProgramASTnode(){
  //   s = "lemon";
  // };
  virtual llvm::Value *codegen(){
    return nullptr;
  };
};
class ExternASTnode : public ASTnode{};
class DeclASTnode : public ASTnode{
public:
  virtual llvm::Value *codegen() = 0;
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
  /// IntASTnode - Class for integer literals like 1, 2, 10,
  int Val;
  TOKEN Tok;
  std::string Name;

public:
  IntASTnode(TOKEN tok, int val) : Tok(tok), Val(val){}
  virtual llvm::Value *codegen() override;
  // virtual std::string to_string() const override {
  //   //return a sting representation of this AST node
  // };
};
class BoolASTnode : public ASTnode{};
class FloatASTnode : public ASTnode{};
class IdentASTnode : public ASTnode{};
class FuncCallASTnode : public ASTnode{};


/* add other AST nodes as nessasary */

