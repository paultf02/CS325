#pragma once
#include "lexer.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <utility>

using std::unique_ptr;
using std::vector;
using std::string;

extern string br;
extern string sp;
extern string nl;

//===----------------------------------------------------------------------===//
// AST nodes
//===----------------------------------------------------------------------===//
// 
/// ASTnode - Base class for all AST nodes.


class BlockASTnode;
class StmtASTnode;
class ExprASTnode;
class AssignASTnode;
class DeclASTnode;
// class ArgListASTnode;
// class DeclListASTnode;

class ASTnode {
public:
  virtual ~ASTnode() {};
  virtual llvm::Value *codegen() = 0;
  virtual string to_string(string pre) const {
    return pre + "not done" + nl;};
};

class BinOpASTnode : public ASTnode{
public:
  TOKEN_TYPE binop;
  unique_ptr<ExprASTnode> lhs;
  unique_ptr<ExprASTnode> rhs;
  BinOpASTnode(TOKEN_TYPE o,
               unique_ptr<ExprASTnode> l,
               unique_ptr<ExprASTnode> r
               ) : binop(o), lhs(std::move(l)), rhs(std::move(r)){};
  virtual llvm::Value *codegen() override{return nullptr;}
  virtual string to_string(string pre) const override;
};

class UnOpASTnode : public ASTnode{
public:
  TOKEN_TYPE unop;
  unique_ptr<ExprASTnode> expr;
  UnOpASTnode(TOKEN_TYPE o,
              unique_ptr<ExprASTnode> e
              ) : unop(o), expr(std::move(e)){};
  virtual llvm::Value *codegen() override{return nullptr;}
  virtual string to_string(string pre) const override;
};

class IntASTnode : public ASTnode {
public:
  /// IntASTnode - Class for integer literals like 1, 2, 10,
  int val;
  TOKEN tok;
  string name;
  IntASTnode(TOKEN t) : val(std::stoi(t.lexeme)), tok(t){};
  virtual llvm::Value *codegen() override{return nullptr;}
  virtual string to_string(string pre) const override {
    return "";
  };
};

class FloatASTnode : public ASTnode{
public:
  float val;
  TOKEN tok;
  string name;
  FloatASTnode(TOKEN t) : val(std::stof(t.lexeme)), tok(t){};
  virtual llvm::Value *codegen() override{return nullptr;}
  virtual string to_string(string pre) const override {
    return "";
  };
};

class BoolASTnode : public ASTnode{
public:
  bool val;
  TOKEN tok;
  string name;
  BoolASTnode(TOKEN t) : val(noerr_str_to_bool(t.lexeme)), tok(t) {};
  virtual llvm::Value *codegen() override{return nullptr;}
  virtual string to_string(string pre) const override {
    return "";
  };
};

class IdentASTnode : public ASTnode{
public:
  TOKEN tok;
  string name;
  IdentASTnode(TOKEN t) : tok(t), name(t.lexeme){};
  virtual llvm::Value *codegen() override{return nullptr;}
  virtual string to_string(string pre) const override {
    return "";
  };
};

class FunCallASTnode : public ASTnode{
public:
  unique_ptr<IdentASTnode> ident;
  // unique_ptr<ArgListASTnode> arglist;
  vector<unique_ptr<ExprASTnode>> arglist;
  // FunCallASTnode(unique_ptr<IdentASTnode> i,
  //                unique_ptr<ArgListASTnode> a
  //                ) : ident(std::move(i)), arglist(std::move(a)){};

  FunCallASTnode(unique_ptr<IdentASTnode> i,
                 vector<unique_ptr<ExprASTnode>> &al){
    ident = std::move(i);
    for (auto &elem : al){
      arglist.push_back(std::move(elem));
    }
  }

  virtual llvm::Value *codegen() override{return nullptr;}
  virtual string to_string(string pre) const override;
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
  unique_ptr<IdentASTnode> ident;
  // string name;
  VarDeclASTnode(unique_ptr<VarTypeASTnode> vt,
                 unique_ptr<IdentASTnode> id
                 ) : vartype(std::move(vt)), ident(std::move(id)){};
  virtual llvm::Value *codegen() override {return nullptr;}
  virtual string to_string(string pre) const override {
    string npre = pre + sp;
    string ans = "";
    ans += pre + "VarDeclASTnode: " + vartype->vartype + sp + ident-> name + nl;
    return ans;
  };
};

class ExprASTnode : public ASTnode{
public:
  string type; // assign, binop, unop, ident, funcall, intlit, floatlit, boollit
  unique_ptr<AssignASTnode> assign;
  unique_ptr<BinOpASTnode> binop;
  unique_ptr<UnOpASTnode> unop;
  unique_ptr<IdentASTnode> ident;
  unique_ptr<FunCallASTnode> funcall;
  unique_ptr<IntASTnode> intlit;
  unique_ptr<FloatASTnode> floatlit;
  unique_ptr<BoolASTnode> boollit;
  ExprASTnode(unique_ptr<AssignASTnode> a) : type("assign"), assign(std::move(a)){};
  ExprASTnode(unique_ptr<BinOpASTnode> b) : type("binop"), binop(std::move(b)){};
  ExprASTnode(unique_ptr<UnOpASTnode> u) : type("unop"), unop(std::move(u)){};
  ExprASTnode(unique_ptr<IdentASTnode> i) : type("ident"), ident(std::move(i)){};
  ExprASTnode(unique_ptr<FunCallASTnode> f) : type("funcall"), funcall(std::move(f)){};
  ExprASTnode(unique_ptr<IntASTnode> in) : type("intlit"), intlit(std::move(in)){};
  ExprASTnode(unique_ptr<FloatASTnode> fl) : type("floatlit"), floatlit(std::move(fl)){};
  ExprASTnode(unique_ptr<BoolASTnode> bo) : type("boollit"), boollit(std::move(bo)){};
  virtual llvm::Value *codegen() override {return nullptr;};
  virtual string to_string(string pre) const override; 
};

class AssignASTnode : public ASTnode{
public:
  unique_ptr<IdentASTnode> ident;
  unique_ptr<ExprASTnode> rhs;
  AssignASTnode(unique_ptr<IdentASTnode> i,
                unique_ptr<ExprASTnode> r
                ) : ident(std::move(i)) , rhs(std::move(r)){};
  virtual llvm::Value *codegen() override {return nullptr;};
  virtual string to_string(string pre) const override;
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
  string whichtype; // "expr_stmt" "block" "if_stmt" "while_stmt" "return_stmt"
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
  virtual string to_string(string pre) const override;
};

class BlockASTnode : public ASTnode{
public:
  // unique_ptr<DeclListASTnode> localdecls;
  vector<unique_ptr<DeclASTnode>> localdecls;
  // unique_ptr<StmtListASTnode> stmtlist;
  vector<unique_ptr<StmtASTnode>> stmtlist;
  // BlockASTnode(unique_ptr<DeclListASTnode> ld,
  //              unique_ptr<StmtListASTnode> sl
  //              ) : localdecls(std::move(ld)), stmtlist(std::move(sl)){}; 
  BlockASTnode(vector<unique_ptr<DeclASTnode>> &ld,
               vector<unique_ptr<StmtASTnode>> &sl){
    for (auto &elem : ld){
      localdecls.push_back(std::move(elem));
    }
    for (auto &elem : sl){
      stmtlist.push_back(std::move(elem));
    }
  }
  
  virtual llvm::Value *codegen() override {return nullptr;}
  virtual string to_string(string pre) const override;
};

class ParamASTnode : public ASTnode{
public:
  unique_ptr<VarTypeASTnode> vartype;
  unique_ptr<IdentASTnode> ident;
  //string ident;
  ParamASTnode(unique_ptr<VarTypeASTnode> vt,
              unique_ptr<IdentASTnode> id
              ) : vartype(std::move(vt)), ident(std::move(id)){};
  virtual llvm::Value *codegen() override {return nullptr;}
  virtual string to_string(string pre) const override {
    string npre = pre + sp;
    string ans = pre + "ParamASTnode " + vartype->vartype + sp + ident->name;
        
    return ans;
  };

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
  virtual string to_string(string pre) const override {
    string npre = pre + sp;
    string ans = pre + "ParamListASTnode" + nl;
    for (auto &p : paramlist){
      ans += pre + p->to_string(npre) + nl;
    }
    ans.pop_back();
    
    return ans;
  };
};

class TypeSpecASTnode : public ASTnode{
public:
  bool isVoid = false;
  unique_ptr<VarTypeASTnode> vartype;
  TypeSpecASTnode(string v) : isVoid(true){};
  TypeSpecASTnode(unique_ptr<VarTypeASTnode> vt) : vartype(std::move(vt)){};
  virtual llvm::Value *codegen() override {return nullptr;}
};

class FunProtoASTnode : public ASTnode{
public:
  unique_ptr<TypeSpecASTnode> typespec;
  unique_ptr<IdentASTnode> ident;
  unique_ptr<ParamListASTnode> params;
  FunProtoASTnode(unique_ptr<TypeSpecASTnode> ts,
                unique_ptr<IdentASTnode> id,
                unique_ptr<ParamListASTnode> ps
                ) : typespec(std::move(ts)), ident(std::move(id)), params(std::move(ps)){};
  virtual llvm::Value *codegen() override {return nullptr;}
  virtual string to_string(string pre) const override {
    string npre = pre + sp;
    string ans = "";
    ans += pre + "FunProtoASTnode: ";
    ans += typespec->vartype->vartype + sp + ident->name;
    ans += "(";
    for (auto &elem : params->paramlist){
      ans += elem->vartype->vartype + ", ";
    }
    ans.pop_back();
    ans.pop_back();
    ans += ")" + nl;
    return ans;
  };
};

class FunDeclASTnode : public ASTnode{
public:
  unique_ptr<FunProtoASTnode> funproto;
  unique_ptr<BlockASTnode> funbody;
  FunDeclASTnode( unique_ptr<TypeSpecASTnode> ts,
                  unique_ptr<IdentASTnode> id,
                  unique_ptr<ParamListASTnode> ps,
                  unique_ptr<BlockASTnode> b){
  funproto = std::make_unique<FunProtoASTnode>(std::move(ts), std::move(id), std::move(ps));
  funbody = std::move(b);
  };
  virtual llvm::Value *codegen() override {return nullptr;}
  virtual string to_string(string pre) const override {
    string npre = pre + sp;
    string ans = "";
    ans += pre + "FunDeclASTnode:" + nl;
    ans += funproto->to_string(npre);
    ans += funbody->to_string(npre);
    return ans;
  };
};

class ExternASTnode : public ASTnode{
public:
  unique_ptr<FunProtoASTnode> funproto;
  ExternASTnode(unique_ptr<TypeSpecASTnode> ts,
                unique_ptr<IdentASTnode> id,
                unique_ptr<ParamListASTnode> ps){
  funproto = std::make_unique<FunProtoASTnode>(std::move(ts), std::move(id), std::move(ps));
  };
  virtual llvm::Value *codegen() override {return nullptr;}
  virtual string to_string(string pre) const override {
    string npre = pre + sp;
    string ans = "";
    ans += pre + "ExternASTnode: " + nl;
    ans += funproto->to_string(npre);
    return ans;
  };
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
  virtual string to_string(string pre) const override {
    string npre = pre;
    string ans = "";
    if (isVar){
      ans = vardecl->to_string(npre);
    } else {
      ans = fundecl->to_string(npre);
    }
    return ans;
  };
};

class ProgramASTnode : public ASTnode{
public:
  vector<unique_ptr<ExternASTnode>> externs;
  vector<unique_ptr<DeclASTnode>> decls;
  // unique_ptr<ExternListASTnode> externlist;
  // unique_ptr<DeclListASTnode> decllist;
  // string s = "hi";
  // ProgramASTnode(unique_ptr<ExternListASTnode> el, 
  //                unique_ptr<DeclListASTnode> dl) : externlist(std::move(el)), decllist(std::move(dl)){}
  ProgramASTnode(vector<unique_ptr<ExternASTnode>> &el,
                 vector<unique_ptr<DeclASTnode>> &dl){
    for (auto &elem : el){
      externs.push_back(std::move(elem));
    }
    for (auto &elem : dl){
      decls.push_back(std::move(elem));
    }
  }

  virtual llvm::Value *codegen() override {
    return nullptr;
  };
  virtual string to_string(string pre) const override {
    string npre = pre + sp;
    string ans = pre + "ProgramASTnode:" + nl;
    for (auto &ext : externs){
      ans += ext->to_string(npre);
    }
    for (auto &dec : decls){
      ans += dec->to_string(npre);
    }
    // ans += pre + externlist->to_string(npre) + nl;
    // ans += pre + decllist->to_string(npre);
    return ans;
  };
};


  // virtual string to_string(string pre) const override {
  //   string npre = pre + sp;
  //   string ans = pre + "BlockASTnode:" + nl;
  //   ans += npre + "local decls here" + nl;
  //   for (auto &ld : localdecls){
  //     ans += ld->to_string(npre);
  //   }
  //   for (auto &stmt : stmtlist){
  //     ans += stmt->to_string(npre);
  //   }    
  //   return ans;
  // };

// class FunDeclASTnode : public ASTnode{
// public:
//   unique_ptr<TypeSpecASTnode> typespec;
//   //string ident;
//   unique_ptr<IdentASTnode> ident;
//   unique_ptr<ParamListASTnode> params;
//   unique_ptr<BlockASTnode> block;
//   FunDeclASTnode( unique_ptr<TypeSpecASTnode> ts,
//                   unique_ptr<IdentASTnode> id,
//                   unique_ptr<ParamListASTnode> ps,
//                   unique_ptr<BlockASTnode> b
//                   ): typespec(std::move(ts)),
//                   ident(std::move(id)),
//                   params(std::move(ps)),
//                   block(std::move(b)) {};
//   virtual llvm::Value *codegen() override {return nullptr;}
//   virtual string to_string(string pre) const override {
//     string npre = pre + sp;
//     string ans = "";
//     ans += pre + "FunDeclASTnode: ";
//     ans += typespec->vartype->vartype + sp + ident->name;
//     ans += "(";
//     for (auto &elem : params->paramlist){
//       ans += elem->vartype->vartype + ", ";
//     }
//     ans.pop_back();
//     ans.pop_back();
//     ans += ")" + nl;
//     return ans;
//   };
// };

// class ExternASTnode : public ASTnode{
// public:
//   unique_ptr<TypeSpecASTnode> typespec;
//   unique_ptr<IdentASTnode> ident;
//   // string ident;
//   unique_ptr<ParamListASTnode> params;
//   ExternASTnode(unique_ptr<TypeSpecASTnode> ts,
//                 unique_ptr<IdentASTnode> id,
//                 unique_ptr<ParamListASTnode> ps
//                 ) : typespec(std::move(ts)), ident(std::move(id)), params(std::move(ps)){};
//   virtual llvm::Value *codegen() override {return nullptr;}
//   virtual string to_string(string pre) const override {
//     string npre = pre + sp;
//     string ans = "";
//     ans += pre + "ExternASTnode: ";
//     ans += typespec->vartype->vartype + sp + ident->name;
//     ans += "(";
//     for (auto &elem : params->paramlist){
//       ans += elem->vartype->vartype + ", ";
//     }
//     ans.pop_back();
//     ans.pop_back();
//     ans += ")" + nl;
//     return ans;
//   };
// };

// class ExternListASTnode : public ASTnode{
// public:
//   vector<unique_ptr<ExternASTnode>> externs;
//   ExternListASTnode(vector<unique_ptr<ExternASTnode>> &e){
//     for(int i=0; i<e.size(); i++){
//       externs.push_back(std::move(e.at(i)));
//     }
//   };
//   virtual llvm::Value *codegen() override {return nullptr;};
//   virtual string to_string(string pre) const override {
//     string npre = pre + sp;
//     string ans = "";
//     for (auto &e : externs){
//       ans += e->to_string(npre) + nl;
//     }
//     ans.pop_back();
//     return ans;
//   };
// };

// class DeclListASTnode : public ASTnode{
// public:
//   vector<unique_ptr<DeclASTnode>> decls;
//   DeclListASTnode(vector<unique_ptr<DeclASTnode>> &d){
//     for(int i=0; i<d.size(); i++){
//       decls.push_back(std::move(d.at(i)));
//     }
//   };
//   virtual llvm::Value *codegen() override {return nullptr;}
//   virtual string to_string(string pre) const override {
//     string npre = pre + sp;
//     string ans = "";
//     for (auto &d : decls){
//       ans += pre + d->to_string(npre) + nl;
//     }
//     ans.pop_back();
//     return ans;
//   };
// };

// class StmtListASTnode : public ASTnode{
// public:
//   vector<unique_ptr<StmtASTnode>> stmtlist;
//   StmtListASTnode(vector<unique_ptr<StmtASTnode>> &sl){
//     for(int i=0; i<sl.size(); i++){
//       stmtlist.push_back(std::move(sl.at(i)));
//     }
//   };

//   virtual llvm::Value *codegen() override {return nullptr;}
// };

// class ArgListASTnode: public ASTnode{
// public:
//   vector<unique_ptr<ExprASTnode>> arglist;
//   ArgListASTnode(vector<unique_ptr<ExprASTnode>> &args){
//     for(int i=0; i<args.size(); i++){
//       arglist.push_back(std::move(args.at(i)));
//     }
//   }
//   virtual llvm::Value *codegen() override {return nullptr;}
// };

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

// class ElseASTnode : public ASTnode{
// public:
//   virtual llvm::Value *codegen() override {return nullptr;}
// };