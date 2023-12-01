#include "astnodes.h"
// #include "llvm/IR/Value.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <string>

using std::string;
using namespace llvm;

extern string br;
extern string sp;
extern string nl;

string StmtASTnode::to_string(string pre) const {
  // std::cout << "in StmtASTnode\n";
  int i = 1;
  string npre = pre;
  // string ans = pre + "StmtASTnode: " + whichtype;
  string ans = "this should not be seen";
  if (whichtype=="expr_stmt"){
      ans = expr_stmt->to_string(npre);
  } else if (whichtype=="block"){
      // std::cout<< "beginning of block in stmtast\n";
      ans = block->to_string(npre);
      // ans = npre + "block node" + nl;
      // std::cout<< "end of block in stmtast\n";
  } else if (whichtype=="if_stmt"){
      ans = if_stmt->to_string(npre);
  } else if (whichtype=="while_stmt"){
      ans = while_stmt->to_string(npre);
  } else if (whichtype=="return_stmt"){
      ans = return_stmt->to_string(npre);
  };
  // std::cout<< "end of stmtast\n";
  return ans;
};

string BlockASTnode::to_string(string pre) const {
  string npre = pre + sp;
  string ans = pre + "BlockASTnode:" + nl;
  // ans += npre + "local decls here" + nl;
  for (auto &ld : localdecls){
    ans += ld->to_string(npre);
  }
  for (auto &stmt : stmtlist){
    ans += stmt->to_string(npre);
  }    
  return ans;
};

string ExprASTnode::to_string(string pre) const {
  // std::cout << "in ExprASTnode with type " + type + "\n";
  string ans;
  ans = pre + "ExprASTnode:" + nl;
  string npre = pre + sp;
  if (type == "assign"){
    ans += assign->to_string(npre);
  } else if (type == "binop"){
    ans += binop->to_string(npre);
  } else if (type == "unop"){
    ans += unop->to_string(npre);
  } else if (type == "ident"){
    ans += ident->to_string(npre);
  } else if (type == "funcall"){
    ans += funcall->to_string(npre);
  } else if (type == "intlit"){
    ans += intlit->to_string(npre);
  } else if (type == "floatlit"){
    ans += floatlit->to_string(npre);
  } else if (type == "boollit"){
    ans += boollit->to_string(npre);
  }
  return ans;
};

string FunCallASTnode::to_string(string pre) const {
  string ans;
  ans = pre + "FunCallASTnode:" + nl;
  string npre = pre + sp;
  ans += ident->to_string(npre);
  ans += npre + "Args:" + nl;
  string nnpre = npre + sp;
  for (auto &elem : arglist){
    ans += elem->to_string(nnpre);
  }
  return ans;
};

string BinOpASTnode::to_string(string pre) const {
  string ans = pre + "BinOpASTnode:" + nl;
  string npre = pre + sp;
  string nnpre = npre + sp;
  ans += npre + "Binary Operation: " + tok.lexeme + nl;
  ans += npre + "LHS:" + nl;
  ans += lhs->to_string(nnpre);
  ans += npre + "RHS:" + nl;
  ans += rhs->to_string(nnpre);
  return ans;
};

string UnOpASTnode::to_string(string pre) const {
  string ans = pre + "UnOpASTnode:" + nl;
  string npre = pre + sp;
  ans += npre + "Unary Operation: " + tok.lexeme + nl;
  ans += expr->to_string(npre);
  return ans;
}

string AssignASTnode::to_string(string pre) const {
  // std::cout << "In AssignASTnode\n";
  string ans = pre + "AssignASTnode:" + nl;
  string npre = pre + sp;
  ans += ident->to_string(npre);
  ans += rhs->to_string(npre);
  return ans;
}

string IfASTnode::to_string(string pre) const {
  string ans = pre + "IfASTnode:" + nl;
  string npre = pre + sp;
  string nnpre = npre + sp;
  ans += expr->to_string(npre);
  ans += block->to_string(npre);
  if (else_stmt){
    ans += npre + "ElseStmt:" + nl;
    ans += else_stmt->to_string(nnpre);
  }
  return ans;
};

string WhileASTnode::to_string(string pre) const {
  // std::cout << "in WhileASTnode\n";
  string ans = pre + "WhileASTnode:" + nl;
  string npre = pre + sp;
  // std::cout << "in while loop before printing expr\n";
  ans += expr->to_string(npre);
  // std::cout << "in while loop after printing expr and before printing stmt\n";
  ans += stmt->to_string(npre);
  return ans;
}

string IntASTnode::to_string(string pre) const {
  // std::cout << "in IntASTnode token on lineNo " + std::to_string(tok.lineNo) + nl;
  string ans = pre + "IntASTnode: " + tok.lexeme + nl;
  return ans;
};

string FloatASTnode::to_string(string pre) const {
  string ans = pre + "FloatASTnode: " + tok.lexeme + nl;
  return ans;
};

string BoolASTnode::to_string(string pre) const {
  string ans = pre + "BoolASTnode: " + tok.lexeme + nl;
  return ans;
};

string IdentASTnode::to_string(string pre) const {
  string ans = pre + "IdentASTnode: " + tok.lexeme + nl;
  return ans;
};

string VarTypeASTnode::to_string(string pre) const {
  string ans = pre + "VarTypeASTnode: " + vartype + nl;
  return ans;
}

string VarDeclASTnode::to_string(string pre) const {
  // std::cout << "called VarDeclASTnode\n";
  string npre = pre + sp;
  string ans = "";
  ans += pre + "VarDeclASTnode: " + vartype->vartype + sp + ident->name + nl;
  // std::cout << "before 158\n";
  auto x = ident->tok.lexeme;
  auto y = std::to_string(ident->tok.lineNo);
  // std::cout << "before 161\n";
  // std::cout << ("finished VarDeclASTnode with ident " + x + " on line " + y + "\n");
  return ans;
};

string ReturnASTnode::to_string(string pre) const {
  // std::cout << "in ReturnASTnode\n";
  string ans = pre + "ReturnASTnode:" + nl;
  string npre = pre + sp;
  if (isVoid){
    ans += npre + "void" + nl;
  } else {
    ans += expr->to_string(npre);
  }
  return ans;
};

string ParamASTnode::to_string(string pre) const {
  // std::cout << "in ParamASTnode\n";
  string npre = pre + sp;
  string ans = pre + "ParamASTnode " + vartype->vartype + sp + ident->name;
      
  return ans;
};

string ParamListASTnode::to_string(string pre) const {
  // std::cout << "in ParamListASTnode\n";
  string npre = pre + sp;
  string ans = pre + "ParamListASTnode" + nl;
  for (auto &p : paramlist){
    // std::cout << "in ParamListASTnode for loop\n";
    ans += pre + p->to_string(npre) + nl;
  }
  ans.pop_back();
  // std::cout << ans;
  return ans;
};

string TypeSpecASTnode::to_string(string pre) const {
  string ans = pre + "TypeSpecASTnode: " + get_type() + nl;
  return ans;
}

string FunProtoASTnode::to_string(string pre) const {
  // std::cout << "called FunProtoASTnode\n";
  string npre = pre + sp;
  string ans = "";
  ans += pre + "FunProtoASTnode: ";
  // // std::cout << "just before getting a=typspec->vartype\n";
  // auto a = typespec->vartype;
  // // std::cout << "just before getting a=typspec->vartype\n";
  // auto b = a->vartype;
  // // std::cout << "just before getting name\n";
  // string n_str = ident->name;
  // ans += typespec->vartype->vartype + sp + ident->name;
  ans += typespec->get_type() + sp + ident->name;
  ans += "(";
  // std::cout << "just before iterating\n";
  for (auto &elem : params->paramlist){
    ans += elem->vartype->vartype + ", ";
  }
  ans.pop_back();
  ans.pop_back();
  ans += ")" + nl;
  return ans;
};

string FunBodyASTnode::to_string(string pre) const{
  string npre = pre + sp;
  string ans = "";
  ans += pre + "FunBodyASTnode:" + nl;
  ans += body->to_string(npre);
  return ans;
};

string FunDeclASTnode::to_string(string pre) const {
  // std::cout << "called FunDeclASTnode\n";
  string npre = pre + sp;
  string ans = "";
  ans += pre + "FunDeclASTnode:" + nl;
  ans += funproto->to_string(npre);
  ans += funbody->to_string(npre);
  return ans;
};

string ExternASTnode::to_string(string pre) const {
  string npre = pre + sp;
  string ans = "";
  ans += pre + "ExternASTnode: " + nl;
  ans += funproto->to_string(npre);
  //// std::cout << ans;
  return ans;
};

string DeclASTnode::to_string(string pre) const {
  // std::cout << "called DeclASTnode\n";
  string npre = pre;
  string ans = "";
  if (isVar){
    ans = vardecl->to_string(npre);
  } else {
    ans = fundecl->to_string(npre);
  }
  //std::cout << ans;
  return ans;
};

string ProgramASTnode::to_string(string pre) const {
  string npre = pre + sp;
  string ans = pre + "ProgramASTnode:" + nl;
  for (auto &ext : externs){
    ans += ext->to_string(npre);
  }
  for (auto &dec : decls){
    ans += dec->to_string(npre);
  }
  // ans += "number of decls is " + std::to_string(decls.size()) + nl;
  // ans += pre + externlist->to_string(npre) + nl;
  // ans += pre + decllist->to_string(npre);
  ans.pop_back(); // remove last newline
  return ans;
};

Value* BinOpASTnode::codegen(){};

Value* UnOpASTnode::codegen(){};

Value* IntASTnode::codegen(){};

Value* FloatASTnode::codegen(){};

Value* BoolASTnode::codegen(){};

Value* IdentASTnode::codegen(){};

Value* FunCallASTnode::codegen(){};

Type* VarTypeASTnode::codegen(){
  if (vartype == "bool"){
    return Type::getInt1Ty(*TheContext);
  } else if (vartype == "float"){
    return Type::getFloatTy(*TheContext);
  } else {
    return Type::getInt32Ty(*TheContext);
  }
};

Value* VarDeclASTnode::codegen(){};

Value* ExprASTnode::codegen(){};

Value* AssignASTnode::codegen(){};

Value* WhileASTnode::codegen(){};

Value* ReturnASTnode::codegen(){};

Value* IfASTnode::codegen(){};

Value* StmtASTnode::codegen(){};

Value* BlockASTnode::codegen(){
  return nullptr;
};

Type *ParamASTnode::codegen(){
  return vartype->codegen();
};

vector<Type *> ParamListASTnode::codegen(){
  vector<Type *> ans;
  for (auto &param : paramlist){
    ans.push_back(param->codegen());
  }
  return ans;
};

Type* TypeSpecASTnode::codegen(){
  if (isVoid){
    return Type::getVoidTy(*TheContext);
  } else {
    return vartype->codegen();
  }
};

Function* FunProtoASTnode::codegen(){
  vector<Type *> argtypes = params->codegen();
  Type *outputtype = typespec->codegen();
  FunctionType *ft = FunctionType::get(outputtype, argtypes, false);
  Function *F = Function::Create(ft, Function::ExternalLinkage, ident->name, TheModule.get());
  // Set names for all arguments.
  unsigned idx = 0;
  for (auto &prm : F->args()){
    prm.setName(params->paramlist.at(idx)->ident->name);
    idx++;
  }
  return F;
};

Value* FunBodyASTnode::codegen(){
  return body->codegen();
}

Value* FunDeclASTnode::codegen(){
  Function *TheFunction = TheModule->getFunction(funproto->ident->name);
  if (!TheFunction){
    TheFunction = funproto->codegen();
  }
  if (!TheFunction){
    return nullptr;
  }

  BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  // NamedValues needs to be modified for scope
  NamedValues.clear();
  for (auto &arg : TheFunction->args()) {
    AllocaInst *alloca = CreateEntryBlockAlloca(TheFunction, arg.getName().str());
    Builder->CreateStore(&arg, alloca);
    NamedValues[std::string(arg.getName())] = alloca;
  }

  // this may not be how we want to return the function
  if (Value *RetVal = funbody->codegen()) {
    // Finish off the function.
    Builder->CreateRet(RetVal);
  }
  // Validate the generated code, checking for consistency.
  verifyFunction(*TheFunction);
  return TheFunction;
};

Value* ExternASTnode::codegen(){
  return funproto->codegen();;
};

Value* DeclASTnode::codegen(){
  if (isVar){
    return vardecl->codegen();
  } else {
    return fundecl->codegen();
  }
};

Value* ProgramASTnode::codegen() {
  for (auto &elem : externs){
    elem->codegen();
  }
  for (auto &elem : decls){
    elem->codegen();
  }
  return nullptr; 
};

AllocaInst* CreateEntryBlockAlloca(Function *TheFunction,const std::string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
  TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt32Ty(*TheContext), 0, VarName.c_str());
}
