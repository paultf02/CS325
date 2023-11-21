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
class ProgramASTnode : public ASTnode{};
class ExternASTnode : public ASTnode{};
class VariableDeclASTnode : public ASTnode{};
class FunctionDeclASTnode : public ASTnode{};
class ArgASTnode : public ASTnode{};
class BlockASTnode : public ASTnode{};
class VariableAssignASTnode : public ASTnode{};
class ExprASTnode : public ASTnode{};
class BinOpASTnode : public ASTnode{};
class UnaryOpASTnode : public ASTnode{};
class LiteralASTnode : public ASTnode{};
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
class FunctionCallASTnode : public ASTnode{};


/* add other AST nodes as nessasary */

