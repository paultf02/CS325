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

class VarTypeASTnode : public ASTnode{
public:
  std::string vartype;
  VarTypeASTnode(std::string vt) : vartype(vt) {};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class VarDeclASTnode : public ASTnode{
public:
  std::unique_ptr<VarTypeASTnode> vartype;
  std::string name;
  VarDeclASTnode(std::unique_ptr<VarTypeASTnode> vt, std::string n) : vartype(std::move(vt)), name(n){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class FunDeclASTnode : public ASTnode{
public:
  virtual llvm::Value *codegen() override {return nullptr;}
};

class TypeSpecASTnode : public ASTnode{
public:
  bool isVoid = false;
  std::unique_ptr<VarTypeASTnode> vartype;
  TypeSpecASTnode(std::string v) : isVoid(true){};
  TypeSpecASTnode(std::unique_ptr<VarTypeASTnode> vt) : vartype(std::move(vt)){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ParamsASTnode : public ASTnode{};

class ExternASTnode : public ASTnode{
public:
  std::unique_ptr<TypeSpecASTnode> typespec;
  std::string ident;
  std::unique_ptr<ParamsASTnode> params;
  ExternASTnode(std::unique_ptr<TypeSpecASTnode> ts,
                std::string id,
                std::unique_ptr<ParamsASTnode> ps
                ) : typespec(std::move(ts)), ident(id), params(std::move(ps)){};
  virtual llvm::Value *codegen() override {return nullptr;}
};



class DeclASTnode : public ASTnode{
public:
  bool isVar;
  std::unique_ptr<VarDeclASTnode> vardecl;
  std::unique_ptr<FunDeclASTnode> fundecl;
  DeclASTnode(std::unique_ptr<VarDeclASTnode> vd
             ) : isVar(true), vardecl(std::move(vd)){}
  DeclASTnode(std::unique_ptr<FunDeclASTnode> fd
             ) : isVar(false), fundecl(std::move(fd)){}
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ExternListASTnode : public ASTnode{
public:
  std::vector<std::unique_ptr<ExternASTnode>> externs;
  ExternListASTnode(std::vector<std::unique_ptr<ExternASTnode>> &e){
    for(int i=0; i<e.size(); i++){
      externs.push_back(std::move(e.at(i)));
    }
  };
  virtual llvm::Value *codegen() override {return nullptr;}
};

class DeclListASTnode : public ASTnode{
public:
  std::vector<std::unique_ptr<DeclASTnode>> decls;
  DeclListASTnode(std::vector<std::unique_ptr<DeclASTnode>> &d){
    for(int i=0; i<d.size(); i++){
      decls.push_back(std::move(d.at(i)));
    }
  };
  virtual llvm::Value *codegen() override {return nullptr;}
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







class ParamASTnode : public ASTnode{};

class ParamListASTnode : public ASTnode{};
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
  virtual llvm::Value *codegen() override{return nullptr;}
  // virtual std::string to_string() const override {
  //   //return a sting representation of this AST node
  // };
};
class BoolASTnode : public ASTnode{};
class FloatASTnode : public ASTnode{};
class IdentASTnode : public ASTnode{};
class FuncCallASTnode : public ASTnode{};


/* add other AST nodes as nessasary */

