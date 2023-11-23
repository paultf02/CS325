#include "lexer.h"
#include "astnodes.h"
#include "loaddata.h"
#include "parser.h"
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <memory> // for std::unique_ptr and std::make_unique
#include <utility> // for std::move
#include <stdexcept>

/*
Recursive descent predictive parsing
void A(){
  choose A-production A -> X1X2...Xk // we will use if then else
  for (i=1 to k){
    if (Xi is a nonterminal){
      call procedure Xi();
    } else if (Xi == current input symbol a){
      advance input to the next symbol;
    } else {
      raise error
    }
  }
}
*/

int lhs_to_index(std::string lhs){
  int ans;
  auto it = std::find(nonterminals.begin(), nonterminals.end(), lhs);
  if (it != nonterminals.end()){
    ans = it - nonterminals.begin();
    return ans;
  } else{
    std::string err_msg = "the name " + lhs + " is not in the lhs of the grammar";
    throw std::runtime_error(err_msg);
  }
}

enum TOKEN_TYPE word_to_type(std::string word){
  TOKEN_TYPE type = INVALID;
  if (word=="IDENT"){
    type = IDENT;
  } else if (word=="'='"){
    type = ASSIGN;
  } else if (word=="'{'"){
    type = LBRA;
  } else if (word=="'}'"){
    type = RBRA;
  } else if (word=="'('"){
    type = LPAR;
  } else if (word=="')'"){
    type = RPAR;
  } else if (word=="';'"){
    type = SC;
  } else if (word=="','"){
    type = COMMA;
  } else if (word=="'int'"){
    type = INT_TOK;
  } else if (word=="'void'"){
    type = VOID_TOK;
  } else if (word=="'float'"){
    type = FLOAT_TOK;
  } else if (word=="'bool'"){
    type = BOOL_TOK;
  } else if (word=="'extern'"){
    type = EXTERN;
  } else if (word=="'if'"){
    type = IF;
  } else if (word=="'else'"){
    type = ELSE;
  } else if (word=="'while'"){
    type = WHILE;
  } else if (word=="'return'"){
    type = RETURN;
  } else if (word=="INT_LIT"){
    type = INT_LIT;
  } else if (word=="FLOAT_LIT"){
    type = FLOAT_LIT;
  } else if (word=="BOOL_LIT"){
    type = BOOL_LIT;
  } else if (word=="'&&'"){
    type = AND;
  } else if (word=="'||'"){
    type = OR;
  } else if (word=="'+'"){
    type = PLUS;
  } else if (word=="'-'"){
    type = MINUS;
  } else if (word=="'*'"){
    type = ASTERIX;
  } else if (word=="'/'"){
    type = DIV;
  } else if (word=="'%'"){
    type = MOD;
  } else if (word=="'!'"){
    type = NOT;
  } else if (word=="'=='"){
    type = EQ;
  } else if (word=="'!='"){
    type = NE;
  } else if (word=="'<='"){
    type = LE;
  } else if (word=="'<'"){
    type = LT;
  } else if (word=="'>='"){
    type = GE;
  } else if (word=="'>'"){
    type = GT;
  } else if (word=="EOF"){
    type = EOF_TOK;
  }
  return type;
}

std::vector<TOKEN_TYPE> terminals_to_type(std::vector<std::string> list_of_terminals){
  std::vector<enum TOKEN_TYPE> tokvector(list_of_terminals.size());
  for (int i=0; i<list_of_terminals.size();i++){
    tokvector[i] = word_to_type(list_of_terminals[i]);
  }
  return tokvector;
}

std::vector<std::string> find_sentence_first(sentence &sentence){
  // NOTE: THIS WILL CONTAIN DUPLICATES
  // error: this will include an epsilon even when it is not 
  std::vector<std::string> sentence_first;
  for (auto &word : sentence){
    std::vector<std::string> thisfirst = first[word];
    sentence_first.insert(sentence_first.end(), thisfirst.begin(), thisfirst.end());
    if (nullable[word] == false){
      break;
    }
  }
  return sentence_first;
}

bool in_sentence_first(TOKEN &tok, sentence &prod){
  std::vector<std::string> sentencefirstraw = find_sentence_first(prod);
  std::vector<TOKEN_TYPE> sentencefirst = terminals_to_type(sentencefirstraw);

  if (std::find(sentencefirst.begin(), sentencefirst.end(), tok.type) != sentencefirst.end()){
    return true;
  } else {
    return false;
  }

}

std::unique_ptr<ProgramASTnode> parser(){
  // std::string name = "arg_list2";
  // std::cout << "name=" + name + " index=" << lhs_to_index(name) << '\n';
  // return std::move(std::make_unique<ProgramASTnode>());

  auto root = parse_program();
  std::cout << root->s << '\n';
  return std::move(root);
}

// we will only need look ahead if we are parsing a lhs term and curtok as mentioned
// in ./grammars/needlookaheadg9
// decl, return_stmt, expr, rval, rval1, rval2, rval3, rval4, rval5, rval7
// apart from the above exceptions, CurTok will be in exactly one of the first sets of the 
// possible production options
// if not then there is a syntax error
// for a nonterminal that is nullable, we need to check if it has returned a nullptr

// do you need to fill the below in so that we can eat the EOF?
// start -> program EOF
// parse_start(){}

// program -> extern_list decl_list | decl_list
std::unique_ptr<ProgramASTnode> parse_program(){
  // std::vector<std::unique_ptr<ExternASTnode>> externs; // these default to empty vector
  // std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::unique_ptr<ExternListASTnode> externlist = nullptr;
  std::unique_ptr<DeclListASTnode> decllist = nullptr;
  // std::unique_ptr<ProgramASTnode> ans;
  // ans = std::make_unique<ProgramASTnode>();

  sentence prod0 = rhslist[lhs_to_index("program")][0];
  sentence prod1 = rhslist[lhs_to_index("program")][1];

  if (in_sentence_first(CurTok, prod0)){
    // program -> extern_list decl_list
    externlist = parse_extern_list(); // not nullable
    decllist = parse_decl_list(); // not nullable
    // decls = parse_decl_list();
    // ans = std::make_unique<ProgramASTnode>(std::move(externs), std::move(decls));
    // ans = std::make_unique<ProgramASTnode>(externs, decls);
    // ans = std::make_unique<ProgramASTnode>();
    return std::make_unique<ProgramASTnode>(std::move(externlist), std::move(decllist));

  } else if (in_sentence_first(CurTok, prod1)){
    // program -> decl_list
    decllist = parse_decl_list(); // not nullable
    // ans = std::make_unique<ProgramASTnode>(std::move(externs), std::move(decls));
    // ans = std::make_unique<ProgramASTnode>(externs, decls);
    // ans = std::make_unique<ProgramASTnode>();
    return std::make_unique<ProgramASTnode>(std::move(externlist), std::move(decllist));
  } else {
    throw ParseError(CurTok, "could not parse program");
  }
  // return std::move(ans);
  // return ans;
}

// extern_list -> extern extern_list1
std::unique_ptr<ExternListASTnode> parse_extern_list(){
  std::vector<std::unique_ptr<ExternASTnode>> externs;
  std::unique_ptr<ExternListASTnode> externlist1;
  // std::vector<std::unique_ptr<ExternASTnode>> externlist1;
  std::unique_ptr<ExternASTnode> ext;
  // std::unique_ptr<ExternListASTnode> ans;

  sentence prod0 = rhslist[lhs_to_index("extern_list")][0];

  // if (in_sentence_first(CurTok, prod0)){
  //   ext = parse_extern(); // not nullable
  //   externs.push_back(std::move(ext));
  //   externlist1 = parse_extern_list1(); // is nullable
  //   if (externlist1){
  //     for (int i=0; i<externlist1->externs.size(); i++){
  //       externs.push_back(std::move(externlist1->externs.at(i)));
  //     }
  //   }
  //   return std::make_unique<ExternListASTnode>(externs);
  // } else {
  //   throw ParseError(CurTok, "could not parse extern_list");
  // }
    ext = parse_extern(); // not nullable
    externs.push_back(std::move(ext));
    externlist1 = parse_extern_list1(); // is nullable
    if (externlist1){
      for (int i=0; i<externlist1->externs.size(); i++){
        externs.push_back(std::move(externlist1->externs.at(i)));
      }
    }
    return std::make_unique<ExternListASTnode>(externs);
}

// extern_list1 -> extern extern_list1 | epsilon
std::unique_ptr<ExternListASTnode> parse_extern_list1(){
  std::vector<std::unique_ptr<ExternASTnode>> externs;
  std::unique_ptr<ExternListASTnode> externlist1;
  std::unique_ptr<ExternASTnode> ext;
  // std::unique_ptr<ExternListASTnode> ans;

  sentence prod0 = rhslist[lhs_to_index("extern_list1")][0];
  sentence prod1_eps = rhslist[lhs_to_index("extern_list1")][1]; // this is epsilon
  
  if (in_sentence_first(CurTok, prod0)){
    ext = parse_extern(); // not nullable
    externs.push_back(std::move(ext));
    externlist1 = parse_extern_list1(); // is nullable
    if (externlist1){
      for (int i=0; i<externlist1->externs.size(); i++){
        externs.push_back(std::move(externlist1->externs.at(i)));
      }
    }
    return std::make_unique<ExternListASTnode>(externs);
  } else {
    return nullptr;
  }
}

// extern -> 'extern' type_spec IDENT '(' params ')' ';'
std::unique_ptr<ExternASTnode> parse_extern(){
  std::string ident;

  // 0
  if (CurTok.type == EXTERN){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting literal 'extern' but got " + CurTok.lexeme);
  }

  // 1
  std::unique_ptr<TypeSpecASTnode> typespec = parse_type_spec(); // not nullable

  // 2
  if (CurTok.type == IDENT){
    ident = CurTok.lexeme;
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  }

  // 3
  if (CurTok.type == LPAR){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting '(' but got " + CurTok.lexeme);
  }

  // 4
  std::unique_ptr<ParamListASTnode> params = parse_params(); // is nullable

  // 5
  if (CurTok.type == RPAR){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting ')' but got " + CurTok.lexeme);
  }

  //6
  if (CurTok.type == SC){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting ';' but got " + CurTok.lexeme);
  }

  return std::make_unique<ExternASTnode>(std::move(typespec), std::move(ident), std::move(params));
}

// decl_list -> decl decl_list1
std::unique_ptr<DeclListASTnode> parse_decl_list(){
  std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::unique_ptr<DeclListASTnode> decllist1;
  // std::vector<std::unique_ptr<ExternASTnode>> externlist1;
  std::unique_ptr<DeclASTnode> decl;
  // std::unique_ptr<DeclListASTnode> ans;

  sentence prod0 = rhslist[lhs_to_index("decl_list")][0];
  
  // if (in_sentence_first(CurTok, prod0)){
  //   decl = parse_decl();
  //   decls.push_back(std::move(decl));
  //   decllist1 = parse_decl_list1(); // this could be nullptr
  //   if (decllist1){
  //     for (int i=0; i<decllist1->decls.size(); i++){
  //       decls.push_back(std::move(decllist1->decls.at(i)));
  //     }
  //   }

  //   return std::make_unique<DeclListASTnode>(decls);
  // } else {
  //   throw ParseError(CurTok, "could not parse decl_list");
  // }

  decl = parse_decl();
  decls.push_back(std::move(decl));
  decllist1 = parse_decl_list1(); // this could be nullptr
  if (decllist1){
    for (int i=0; i<decllist1->decls.size(); i++){
      decls.push_back(std::move(decllist1->decls.at(i)));
    }
  }

  return std::make_unique<DeclListASTnode>(decls);
}

// decl_list1 -> decl decl_list1 | epsilon
std::unique_ptr<DeclListASTnode> parse_decl_list1(){
  std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::unique_ptr<DeclListASTnode> decllist1;
  // std::vector<std::unique_ptr<ExternASTnode>> externlist1;
  std::unique_ptr<DeclASTnode> decl;
  // std::unique_ptr<DeclListASTnode> ans;

  sentence prod0 = rhslist[lhs_to_index("decl_list1")][0];
  sentence prod1_eps = rhslist[lhs_to_index("extern_list1")][1]; // this is epsilon
  

  if (in_sentence_first(CurTok, prod0)){
    decl = parse_decl();
    decls.push_back(std::move(decl));
    decllist1 = parse_decl_list1();
    for (int i=0; i<decllist1->decls.size(); i++){
      decls.push_back(std::move(decllist1->decls.at(i)));
    }
    return std::make_unique<DeclListASTnode>(decls);
  } else {
    return nullptr;
  }
}

// decl -> var_decl | fun_decl
std::unique_ptr<DeclASTnode> parse_decl(){
  sentence prod0 = rhslist[lhs_to_index("decl")][0];
  sentence prod1 = rhslist[lhs_to_index("decl")][1];

  TOKEN tempcur = CurTok;
  if ( !in_sentence_first(tempcur, prod0) && !in_sentence_first(tempcur, prod1)){
    throw ParseError(CurTok, "could not parse decl");
  }
  getNextToken();
  TOKEN temp1 = CurTok;
  if (temp1.type != IDENT){
    throw ParseError(temp1, "was expecting IDENT but got " + temp1.lexeme);
  }

  getNextToken();
  TOKEN temp2 = CurTok;
  if (temp2.type == LPAR){
    putBackToken(temp2);
    putBackToken(temp1);
    putBackToken(tempcur);
    getNextToken(); // CurTok is now back to what it was (same value as tempcur)
    std::unique_ptr<FunDeclASTnode> fundecl = parse_fun_decl(); // not nullable
    return std::make_unique<DeclASTnode>(std::move(fundecl));
  } else if (temp2.type == SC){
    putBackToken(temp2);
    putBackToken(temp1);
    putBackToken(tempcur);
    getNextToken(); // CurTok is now back to what it was (same value as tempcur)
    std::unique_ptr<VarDeclASTnode> vardecl = parse_var_decl(); // not nullable
    return std::make_unique<DeclASTnode>(std::move(vardecl));
  } else {
    if (tempcur.type == VOID_TOK){
      throw ParseError(temp2, "was expecting '(' but got " + temp2.lexeme);
    } else {
      throw ParseError(temp2, "was expecting '(' or ';' but got " + temp2.lexeme);
    }
  }
}

// var_decl -> var_type IDENT ';'
std::unique_ptr<VarDeclASTnode> parse_var_decl(){
  sentence prod0 = rhslist[lhs_to_index("var_decl")][0];
  std::unique_ptr<VarTypeASTnode> vt;
  // if (in_sentence_first(CurTok, prod0)){
  //   std::unique_ptr<VarTypeASTnode> vt = parse_var_type();
  // } else {
  //   throw ParseError(CurTok, "could not parse var_decl");
  // }
  vt = parse_var_type();
  if (CurTok.type != IDENT){
    throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  }
  TOKEN idtok = CurTok;
  getNextToken();
  if (CurTok.type != SC){
    throw ParseError(CurTok, "was expecting ';' but got " + CurTok.lexeme);
  }
  return std::make_unique<VarDeclASTnode>(std::move(vt), CurTok.lexeme);
}

// type_spec -> 'void' | var_type
std::unique_ptr<TypeSpecASTnode> parse_type_spec(){
  // sentence prod0 = rhslist[lhs_to_index("type_spec")][0];
  sentence prod1 = rhslist[lhs_to_index("type_spec")][1];

  if (CurTok.type == VOID_TOK){
    getNextToken();
    return std::make_unique<TypeSpecASTnode>("void");
  } else if (in_sentence_first(CurTok, prod1)) {
    std::unique_ptr<VarTypeASTnode> vartypeptr = parse_var_type();
    return std::make_unique<TypeSpecASTnode>(std::move(vartypeptr));
  } else {
    throw ParseError(CurTok, "could not parse type_spec");
  }
}

// var_type -> 'int' | 'float' | 'bool'
std::unique_ptr<VarTypeASTnode> parse_var_type(){
  if (CurTok.type == INT_TOK){
    getNextToken();
    return std::make_unique<VarTypeASTnode>("int");
  } else if (CurTok.type == FLOAT_TOK){
    getNextToken();
    return std::make_unique<VarTypeASTnode>("float");
  } else if (CurTok.type == BOOL_TOK){
    getNextToken();
    return std::make_unique<VarTypeASTnode>("bool");
  } else {
    throw ParseError(CurTok, "was expecting 'int', 'float' or 'bool' but got " + CurTok.lexeme);
  }
}

// fun_decl -> type_spec IDENT '(' params ')' block
std::unique_ptr<FunDeclASTnode> parse_fun_decl(){
  sentence prod0 = rhslist[lhs_to_index("fun_decl")][0];
  std::unique_ptr<TypeSpecASTnode> typespec;
  std::string ident;
  std::unique_ptr<ParamListASTnode> params;
  std::unique_ptr<BlockASTnode> block;

  // 0
  // if (in_sentence_first(CurTok, prod0)){
  //   typespec = parse_type_spec(); // not nullable
  // } else {
  //   throw ParseError(CurTok, "could not parse fun_decl");
  // }
  typespec = parse_type_spec(); // not nullable

  // 1
  if (CurTok.type == IDENT){
    ident = CurTok.lexeme;
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  }
  // 2
  if (CurTok.type != LPAR){
    throw ParseError(CurTok, "was expecting '(' but got " + CurTok.lexeme);
  } else {
    getNextToken();
  }

  // 3
  params = parse_params(); // is nullable, will return pointer to empty list if no params
  
  // 4
  if (CurTok.type != RPAR){
    throw ParseError(CurTok, "was expecting ')' but got " + CurTok.lexeme);
  } else {
    getNextToken();
  }

  // 5
  block = parse_block();

  return std::make_unique<FunDeclASTnode>(std::move(typespec), ident, std::move(params), std::move(block));
}

// params -> param_list | 'void' | epsilon
std::unique_ptr<ParamListASTnode> parse_params(){
  sentence prod0 = rhslist[lhs_to_index("params")][0];
  std::unique_ptr<ParamListASTnode> paramlist;
  if (in_sentence_first(CurTok, prod0)){
    paramlist = parse_param_list();
    return std::move(paramlist);
  } else if (CurTok.type == VOID_TOK) {
    getNextToken();
    return std::make_unique<ParamListASTnode>();
  } else {
    return std::make_unique<ParamListASTnode>();
  }
}

// param_list -> param param_list1
std::unique_ptr<ParamListASTnode> parse_param_list(){
  std::vector<std::unique_ptr<ParamASTnode>> paramlist;
  std::unique_ptr<ParamListASTnode> paramlist1;
  std::unique_ptr<ParamASTnode> param;

  sentence prod0 = rhslist[lhs_to_index("param_list")][0];

  // if (in_sentence_first(CurTok, prod0)){
  //   param = parse_param(); // not nullable
  //   paramlist.push_back(std::move(param));
  //   paramlist1 = parse_param_list1(); // is nullable
  //   if (paramlist1){
  //     for (int i=0; i<paramlist1->paramlist.size(); i++){
  //       paramlist.push_back(std::move(paramlist1->paramlist.at(i)));
  //     }
  //   }
  //   return std::make_unique<ParamListASTnode>(paramlist);
  // } else {
  //   throw ParseError(CurTok, "could not parse param_list");
  // }

  param = parse_param(); // not nullable
  paramlist.push_back(std::move(param));
  paramlist1 = parse_param_list1(); // is nullable
  if (paramlist1){
    for (int i=0; i<paramlist1->paramlist.size(); i++){
      paramlist.push_back(std::move(paramlist1->paramlist.at(i)));
    }
  }
  return std::make_unique<ParamListASTnode>(paramlist);
}

// param_list1 -> ',' param param_list1 | epsilon
std::unique_ptr<ParamListASTnode> parse_param_list1(){
  std::vector<std::unique_ptr<ParamASTnode>> paramlist;
  std::unique_ptr<ParamASTnode> param;
  std::unique_ptr<ParamListASTnode> paramlist1;
  if (CurTok.type == COMMA){
    getNextToken();
    param = parse_param(); // not nullable
    paramlist.push_back(std::move(param));
    paramlist1 = parse_param_list1(); // is nullable
    if (paramlist1){
      for (int i=0; i<paramlist1->paramlist.size(); i++){
        paramlist.push_back(std::move(paramlist1->paramlist.at(i)));
      }
    }
    return std::make_unique<ParamListASTnode>(paramlist);
  } else {
    return nullptr;
  }
}

// param -> var_type IDENT
std::unique_ptr<ParamASTnode> parse_param(){
  sentence prod0 = rhslist[lhs_to_index("param")][0];
  std::unique_ptr<VarTypeASTnode> vartype;
  std::string ident;
  // if (in_sentence_first(CurTok, prod0)){
  //   vartype = parse_var_type();
  //   if (CurTok.type == IDENT){
  //     ident = CurTok.lexeme;
  //     getNextToken();
  //     return std::make_unique<ParamASTnode>(std::move(vartype), ident);
  //   } else {
  //     throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  //   }
  // } else {
  //   throw ParseError(CurTok, "could not parse param");
  // }
  vartype = parse_var_type();
  if (CurTok.type == IDENT){
    ident = CurTok.lexeme;
    getNextToken();
    return std::make_unique<ParamASTnode>(std::move(vartype), ident);
  } else {
      throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
    }
}

// block -> '{' local_decls stmt_list '}'
std::unique_ptr<BlockASTnode> parse_block(){
  std::unique_ptr<LocalDeclListASTnode> localdecls;
  std::unique_ptr<StmtListASTnode> stmtlist;
  if (CurTok.type == LBRA){
    getNextToken();
    localdecls = parse_local_decls(); // is nullable
    stmtlist = parse_stmt_list();
    return std::make_unique<BlockASTnode>(std::move(localdecls), std::move(stmtlist));
    if (CurTok.type == RBRA){
      getNextToken();
    } else {
      throw ParseError(CurTok, "was expecting '}' but got " + CurTok.lexeme);
    }
  } else {
    throw ParseError(CurTok, "was expecting '{' but got " + CurTok.lexeme);
  }
}

// local_decls -> local_decl local_decls | epsilon
std::unique_ptr<LocalDeclListASTnode> parse_local_decls(){
  sentence prod0 = rhslist[lhs_to_index("local_decls")][0];
  std::vector<std::unique_ptr<VarDeclASTnode>> localdecllist;
  std::unique_ptr<LocalDeclListASTnode> localdecllist1;
  std::unique_ptr<VarDeclASTnode> localdecl;

  if (in_sentence_first(CurTok, prod0)){
    localdecl = parse_local_decl();
    localdecllist.push_back(std::move(localdecl));
    localdecllist1 = parse_local_decls(); // is nullable
    if (localdecllist1){
      for (int i=0; i<localdecllist1->localdecllist.size(); i++){
        localdecllist.push_back(std::move(localdecllist1->localdecllist.at(i)));
      }
    }
    return std::make_unique<LocalDeclListASTnode>(localdecllist);    
  } else {
    return nullptr;
  }
}

// local_decl -> var_type IDENT ';'
std::unique_ptr<VarDeclASTnode> parse_local_decl(){
  sentence prod0 = rhslist[lhs_to_index("local_decl")][0];
  std::unique_ptr<VarTypeASTnode> vartype;
  std::string ident;
  vartype = parse_var_type();
  if (CurTok.type == IDENT){
    ident = CurTok.lexeme;
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  }
  if (CurTok.type == SC){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting ';' but got " + CurTok.lexeme);
  }
  return std::make_unique<VarDeclASTnode>(std::move(vartype), ident);
}

// stmt_list -> stmt stmt_list | epsilon
std::unique_ptr<StmtListASTnode> parse_stmt_list(){
  sentence prod0 = rhslist[lhs_to_index("stmt_list")][0];
  std::unique_ptr<StmtASTnode> stmt;
  std::vector<std::unique_ptr<StmtASTnode>> stmtlist;
  std::unique_ptr<StmtListASTnode> stmtlist1;
  if (in_sentence_first(CurTok, prod0)){
    stmt = parse_stmt();
    stmtlist.push_back(std::move(stmt));
    stmtlist1 = parse_stmt_list(); //is nullable
    if (stmtlist1){
      for (int i=0; i<stmtlist1->stmtlist.size(); i++){
        stmtlist.push_back(std::move(stmtlist1->stmtlist.at(i)));
      }
    }
    return std::make_unique<StmtListASTnode>(stmtlist);
  } else {
    return nullptr;
  }
}

// stmt -> expr_stmt | block | if_stmt | while_stmt | return_stmt
std::unique_ptr<StmtASTnode> parse_stmt(){
  return nullptr;
}

// // expr_stmt -> expr ';' | ';'
// parse_expr_stmt(){}

// // while_stmt -> 'while' '(' expr ')' stmt
// parse_while_stmt(){}

// // if_stmt -> 'if' '(' expr ')' block else_stmt
// parse_if_stmt(){}

// // else_stmt -> 'else' block | epsilon
// parse_else_stmt(){}

// // return_stmt -> 'return' ';' | 'return' expr ';'
// parse_return_stmt(){}

// // expr -> IDENT '=' expr | rval
// parse_expr(){}

// // rval -> rval1 '||' rval | rval1
// parse_rval(){}
// // rval1 -> rval2 '&&' rval1 | rval2
// parse_rval1(){}
// // rval2 -> rval3 '==' rval2 | rval3 '!=' rval2 | rval3
// parse_rval2(){}
// // rval3 -> rval4 '<=' rval3 | rval4 '<' rval3 | rval4 '>=' rval3 | rval4 '>' rval3 | rval4
// parse_rval3(){}
// // rval4 -> rval5 '+' rval4 | rval5 '-' rval4 | rval5
// parse_rval4(){}
// // rval5 -> rval6 '*' rval5 | rval6 '/' rval5 | rval6 '%' rval5 | rval6
// parse_rval5(){}
// // rval6 -> '-' rval6 | '!' rval6 | rval7
// parse_rval6(){}
// // rval7 -> '(' expr ')' | IDENT | IDENT '(' args ')' | INT_LIT | FLOAT_LIT | BOOL_LIT
// parse_rval7(){}

// // args -> arg_list | epsilon
// parse_args(){}

// // arg_list -> expr arg_list1
// parse_args_list(){}

// // arg_list1 -> ',' expr arg_list1 | epsilon
// parse_args_list1(){}



