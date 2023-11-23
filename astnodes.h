#pragma once
#include "lexer.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <utility>

using std::unique_ptr;
using std::vector;
using std::string;

//===----------------------------------------------------------------------===//
// AST nodes
//===----------------------------------------------------------------------===//
// 
/// ASTnode - Base class for all AST nodes.
class ASTnode {
public:
  virtual ~ASTnode() {};
  virtual llvm::Value *codegen() = 0;
  virtual string to_string() const {return "";};
};

class BlockASTnode;
class DeclListASTnode;
class StmtASTnode;
class ExprASTnode;

class ArgListASTnode: public ASTnode{
public:
  vector<unique_ptr<ExprASTnode>> arglist;
  ArgListASTnode(vector<unique_ptr<ExprASTnode>> &args){
    for(int i=0; i<args.size(); i++){
      arglist.push_back(std::move(args.at(i)));
    }
  }
  virtual llvm::Value *codegen() override {return nullptr;}
};

class VarTypeASTnode : public ASTnode{
public:
  string vartype;
  VarTypeASTnode(string vt) : vartype(vt) {};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class VarDeclASTnode : public ASTnode{
public:
  unique_ptr<VarTypeASTnode> vartype;
  string name;
  VarDeclASTnode(unique_ptr<VarTypeASTnode> vt, string n) : vartype(std::move(vt)), name(n){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

// class LocalDeclASTnode : public ASTnode{
// public:
//   unique_ptr<VarTypeASTnode> vartype;
//   string name;
//   LocalDeclASTnode(unique_ptr<VarTypeASTnode> vt, string n) : vartype(std::move(vt)), name(n){};
//   virtual llvm::Value *codegen() override {return nullptr;}
// };

// class LocalDeclListASTnode : public ASTnode{
// public:
//   vector<unique_ptr<VarDeclASTnode>> localdecllist;
//   LocalDeclListASTnode(vector<unique_ptr<VarDeclASTnode>> &ld){
//     for(int i=0; i<ld.size(); i++){
//       localdecllist.push_back(std::move(ld.at(i)));
//     }
//   };
//   virtual llvm::Value *codegen() override {return nullptr;}
// };

class ExprASTnode : public ASTnode{
public:
  ExprASTnode(){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class WhileASTnode : public ASTnode{
public:
  unique_ptr<ExprASTnode> expr;
  unique_ptr<StmtASTnode> stmt;
  WhileASTnode(unique_ptr<ExprASTnode> e,
               unique_ptr<StmtASTnode> s) : expr(std::move(e)) , stmt(std::move(s)){}
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ReturnASTnode : public ASTnode{
public:
  unique_ptr<ExprASTnode> expr;
  ReturnASTnode(unique_ptr<ExprASTnode> e) : expr(std::move(e)){};
  ReturnASTnode(){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

// class ElseASTnode : public ASTnode{
// public:
//   virtual llvm::Value *codegen() override {return nullptr;}
// };

class IfASTnode : public ASTnode{
public:
  unique_ptr<ExprASTnode> expr;
  unique_ptr<BlockASTnode> block;
  unique_ptr<BlockASTnode> else_stmt; //nullptr if no else
  IfASTnode(unique_ptr<ExprASTnode> e,
            unique_ptr<BlockASTnode> b,
            unique_ptr<BlockASTnode> e_stmt
            ) : expr(std::move(e)) , block(std::move(b)), else_stmt(std::move(e_stmt)){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class StmtASTnode : public ASTnode{
public:
  string whichtype;
  unique_ptr<ExprASTnode> expr_stmt;
  unique_ptr<BlockASTnode> block;
  unique_ptr<IfASTnode> if_stmt;
  unique_ptr<WhileASTnode> while_stmt;
  unique_ptr<ReturnASTnode> return_stmt;
  StmtASTnode(unique_ptr<ExprASTnode> e) : whichtype("expr_stmt"), expr_stmt(std::move(e)) {};
  StmtASTnode(unique_ptr<BlockASTnode> b) : whichtype("block"), block(std::move(b)) {}; 
  StmtASTnode(unique_ptr<IfASTnode> i) : whichtype("if_stmt"), if_stmt(std::move(i)) {}; 
  StmtASTnode(unique_ptr<WhileASTnode> w) : whichtype("while_stmt"), while_stmt(std::move(w)) {}; 
  StmtASTnode(unique_ptr<ReturnASTnode> r) : whichtype("return_stmt"), return_stmt(std::move(r)) {};  
  virtual llvm::Value *codegen() override {return nullptr;}
};

class StmtListASTnode : public ASTnode{
public:
  vector<unique_ptr<StmtASTnode>> stmtlist;
  StmtListASTnode(vector<unique_ptr<StmtASTnode>> &sl){
    for(int i=0; i<sl.size(); i++){
      stmtlist.push_back(std::move(sl.at(i)));
    }
  };

  virtual llvm::Value *codegen() override {return nullptr;}
};

class BlockASTnode : public ASTnode{
public:
  unique_ptr<DeclListASTnode> localdecls;
  unique_ptr<StmtListASTnode> stmtlist;
  BlockASTnode(unique_ptr<DeclListASTnode> ld,
               unique_ptr<StmtListASTnode> sl
               ) : localdecls(std::move(ld)), stmtlist(std::move(sl)){}; 
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ParamASTnode : public ASTnode{
public:
  unique_ptr<VarTypeASTnode> vartype;
  string ident;
  ParamASTnode(unique_ptr<VarTypeASTnode> vt,
              string id) : vartype(std::move(vt)), ident(id){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ParamListASTnode : public ASTnode{
public:
  vector<unique_ptr<ParamASTnode>> paramlist;
  ParamListASTnode(vector<unique_ptr<ParamASTnode>> &pl){
    for(int i=0; i<pl.size(); i++){
      paramlist.push_back(std::move(pl.at(i)));
    }
  };
  ParamListASTnode(){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class TypeSpecASTnode : public ASTnode{
public:
  bool isVoid = false;
  unique_ptr<VarTypeASTnode> vartype;
  TypeSpecASTnode(string v) : isVoid(true){};
  TypeSpecASTnode(unique_ptr<VarTypeASTnode> vt) : vartype(std::move(vt)){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class FunDeclASTnode : public ASTnode{
public:
  unique_ptr<TypeSpecASTnode> typespec;
  string ident;
  unique_ptr<ParamListASTnode> params;
  unique_ptr<BlockASTnode> block;
  FunDeclASTnode( unique_ptr<TypeSpecASTnode> ts,
                  string id,
                  unique_ptr<ParamListASTnode> ps,
                  unique_ptr<BlockASTnode> b
                  ): typespec(std::move(ts)),
                  ident(id),
                  params(std::move(ps)),
                  block(std::move(b)) {};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ExternASTnode : public ASTnode{
public:
  unique_ptr<TypeSpecASTnode> typespec;
  string ident;
  unique_ptr<ParamListASTnode> params;
  ExternASTnode(unique_ptr<TypeSpecASTnode> ts,
                string id,
                unique_ptr<ParamListASTnode> ps
                ) : typespec(std::move(ts)), ident(id), params(std::move(ps)){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class DeclASTnode : public ASTnode{
public:
  bool isVar;
  unique_ptr<VarDeclASTnode> vardecl;
  unique_ptr<FunDeclASTnode> fundecl;
  DeclASTnode(unique_ptr<VarDeclASTnode> vd
             ) : isVar(true), vardecl(std::move(vd)){}
  DeclASTnode(unique_ptr<FunDeclASTnode> fd
             ) : isVar(false), fundecl(std::move(fd)){}
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ExternListASTnode : public ASTnode{
public:
  vector<unique_ptr<ExternASTnode>> externs;
  ExternListASTnode(vector<unique_ptr<ExternASTnode>> &e){
    for(int i=0; i<e.size(); i++){
      externs.push_back(std::move(e.at(i)));
    }
  };
  virtual llvm::Value *codegen() override {return nullptr;}
};

class DeclListASTnode : public ASTnode{
public:
  vector<unique_ptr<DeclASTnode>> decls;
  DeclListASTnode(vector<unique_ptr<DeclASTnode>> &d){
    for(int i=0; i<d.size(); i++){
      decls.push_back(std::move(d.at(i)));
    }
  };
  virtual llvm::Value *codegen() override {return nullptr;}
};

class ProgramASTnode : public ASTnode{
public:
  // vector<unique_ptr<ExternASTnode>> externs;
  // vector<unique_ptr<DeclASTnode>> decls;
  unique_ptr<ExternListASTnode> externlist;
  unique_ptr<DeclListASTnode> decllist;
  string s = "hi";
  ProgramASTnode(unique_ptr<ExternListASTnode> el, 
                 unique_ptr<DeclListASTnode> dl) : externlist(std::move(el)), decllist(std::move(dl)){}

  virtual llvm::Value *codegen() override {return nullptr;}
};





class BinOpASTnode : public ASTnode{};
class UnOpASTnode : public ASTnode{};

class IntASTnode : public ASTnode {
public:
  /// IntASTnode - Class for integer literals like 1, 2, 10,
  int Val;
  TOKEN Tok;
  string Name;
  IntASTnode(TOKEN tok, int val) : Tok(tok), Val(val){}
  virtual llvm::Value *codegen() override{return nullptr;}
  // virtual string to_string() const override {
  //   //return a sting representation of this AST node
  // };
};
class BoolASTnode : public ASTnode{};
class FloatASTnode : public ASTnode{};
class IdentASTnode :public ASTnode{};

class FunCallASTnode : public ASTnode{};


/* add other AST nodes as nessasary */

