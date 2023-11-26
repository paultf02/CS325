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

using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::string; 

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

int lhs_to_index(string lhs){
  int ans;
  auto it = std::find(nonterminals.begin(), nonterminals.end(), lhs);
  if (it != nonterminals.end()){
    ans = it - nonterminals.begin();
    return ans;
  } else{
    string err_msg = "the name " + lhs + " is not in the lhs of the grammar";
    throw std::runtime_error(err_msg);
  }
}

enum TOKEN_TYPE word_to_type(string word){
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

vector<TOKEN_TYPE> terminals_to_type(vector<string> list_of_terminals){
  vector<enum TOKEN_TYPE> tokvector(list_of_terminals.size());
  for (int i=0; i<list_of_terminals.size();i++){
    tokvector[i] = word_to_type(list_of_terminals[i]);
  }
  return tokvector;
}

vector<string> find_sentence_first(sentence &sentence){
  // NOTE: THIS WILL CONTAIN DUPLICATES
  // error: this will include an epsilon even when it is not 
  vector<string> sentence_first;
  for (auto &word : sentence){
    vector<string> thisfirst = first[word];
    sentence_first.insert(sentence_first.end(), thisfirst.begin(), thisfirst.end());
    if (nullable[word] == false){
      break;
    }
  }
  return sentence_first;
}

bool in_sentence_first(TOKEN &tok, sentence &prod){
  vector<string> sentencefirstraw = find_sentence_first(prod);
  vector<TOKEN_TYPE> sentencefirst = terminals_to_type(sentencefirstraw);

  if (std::find(sentencefirst.begin(), sentencefirst.end(), tok.type) != sentencefirst.end()){
    return true;
  } else {
    return false;
  }

}

unique_ptr<ProgramASTnode> parser(){
  // string name = "arg_list2";
  // std::cout << "name=" + name + " index=" << lhs_to_index(name) << '\n';
  // return std::move(make_unique<ProgramASTnode>());

  auto root = parse_program();
  // std::cout << root->s << '\n';
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
unique_ptr<ProgramASTnode> parse_program(){
  vector<unique_ptr<ExternASTnode>> externs; // these default to empty vector
  vector<unique_ptr<DeclASTnode>> decls;
  // unique_ptr<ExternListASTnode> externlist = nullptr;
  // unique_ptr<DeclListASTnode> decllist = nullptr;

  // unique_ptr<ProgramASTnode> ans;
  // ans = make_unique<ProgramASTnode>();

  sentence prod0 = rhslist[lhs_to_index("program")][0];
  sentence prod1 = rhslist[lhs_to_index("program")][1];

  if (in_sentence_first(CurTok, prod0)){
    // program -> extern_list decl_list
    // externlist = parse_extern_list(); // not nullable
    // decllist = parse_decl_list(); // not nullable
    
    parse_extern_list(externs); //populate externs
    parse_decl_list(decls); //populate decls

    // decls = parse_decl_list();
    // ans = make_unique<ProgramASTnode>(std::move(externs), std::move(decls));
    // ans = make_unique<ProgramASTnode>(externs, decls);
    // ans = make_unique<ProgramASTnode>();
    // return make_unique<ProgramASTnode>(std::move(externlist), std::move(decllist));
    return make_unique<ProgramASTnode>(externs, decls);

  } else if (in_sentence_first(CurTok, prod1)){
    // program -> decl_list
    // decllist = parse_decl_list(); // not nullable
    parse_decl_list(decls);
    // ans = make_unique<ProgramASTnode>(std::move(externs), std::move(decls));
    // ans = make_unique<ProgramASTnode>(externs, decls);
    // ans = make_unique<ProgramASTnode>();
    // return make_unique<ProgramASTnode>(std::move(externlist), std::move(decllist));
    return make_unique<ProgramASTnode>(externs, decls);

  } else {
    throw ParseError(CurTok, "could not find production for program");
  }
  // return std::move(ans);
  // return ans;
}

// extern_list -> extern extern_list1
// unique_ptr<ExternListASTnode> parse_extern_list(){
//   vector<unique_ptr<ExternASTnode>> externs;
//   unique_ptr<ExternListASTnode> externlist1;
//   // vector<unique_ptr<ExternASTnode>> externlist1;
//   unique_ptr<ExternASTnode> ext;
//   // unique_ptr<ExternListASTnode> ans;
//   sentence prod0 = rhslist[lhs_to_index("extern_list")][0];

//   // if (in_sentence_first(CurTok, prod0)){
//   //   ext = parse_extern(); // not nullable
//   //   externs.push_back(std::move(ext));
//   //   externlist1 = parse_extern_list1(); // is nullable
//   //   if (externlist1){
//   //     for (int i=0; i<externlist1->externs.size(); i++){
//   //       externs.push_back(std::move(externlist1->externs.at(i)));
//   //     }
//   //   }
//   //   return make_unique<ExternListASTnode>(externs);
//   // } else {
//   //   throw ParseError(CurTok, "could not find production for extern_list");
//   // }
//     ext = parse_extern(); // not nullable
//     externs.push_back(std::move(ext));
//     externlist1 = parse_extern_list1(); // is nullable
//     if (externlist1){
//       for (int i=0; i<externlist1->externs.size(); i++){
//         externs.push_back(std::move(externlist1->externs.at(i)));
//       }
//     }
//     return make_unique<ExternListASTnode>(externs);
// }

void parse_extern_list(vector<unique_ptr<ExternASTnode>> &externs){
  ;
  // unique_ptr<ExternListASTnode> externlist1;
  // vector<unique_ptr<ExternASTnode>> externlist1;
  //  ext;
  // unique_ptr<ExternListASTnode> ans;
  unique_ptr<ExternASTnode> ext = parse_extern(); // not nullable
  externs.push_back(std::move(ext));
  parse_extern_list1(externs);
    // parse_extern_list1(externlist1); // is nullable
    // if (externlist1){
    //   for (int i=0; i<externlist1->externs.size(); i++){
    //     externs.push_back(std::move(externlist1->externs.at(i)));
    //   }
    // }
    //return make_unique<ExternListASTnode>(externs);
}

// extern_list1 -> extern extern_list1 | epsilon
unique_ptr<ExternListASTnode> parse_extern_list1(){
  vector<unique_ptr<ExternASTnode>> externs;
  unique_ptr<ExternListASTnode> externlist1;
  unique_ptr<ExternASTnode> ext;
  // unique_ptr<ExternListASTnode> ans;

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
    return make_unique<ExternListASTnode>(externs);
  } else {
    return nullptr;
  }
}

// extern -> 'extern' type_spec IDENT '(' params ')' ';'
unique_ptr<ExternASTnode> parse_extern(){
  // string ident;
  unique_ptr<IdentASTnode> ident;

  // 0
  if (CurTok.type == EXTERN){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting literal 'extern' but got " + CurTok.lexeme);
  }

  // 1
  unique_ptr<TypeSpecASTnode> typespec = parse_type_spec(); // not nullable

  // 2
  if (CurTok.type == IDENT){
    // ident = CurTok.lexeme;
    ident = make_unique<IdentASTnode>(CurTok);
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
  unique_ptr<ParamListASTnode> params = parse_params(); // is nullable

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

  return make_unique<ExternASTnode>(std::move(typespec), std::move(ident), std::move(params));
}

// decl_list -> decl decl_list1
unique_ptr<DeclListASTnode> parse_decl_list(){
  vector<unique_ptr<DeclASTnode>> decls;
  unique_ptr<DeclListASTnode> decllist1;
  // vector<unique_ptr<ExternASTnode>> externlist1;
  unique_ptr<DeclASTnode> decl;
  // unique_ptr<DeclListASTnode> ans;

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

  //   return make_unique<DeclListASTnode>(decls);
  // } else {
  //   throw ParseError(CurTok, "could not find production for decl_list");
  // }

  decl = parse_decl();
  decls.push_back(std::move(decl));
  decllist1 = parse_decl_list1(); // this could be nullptr
  if (decllist1){
    for (int i=0; i<decllist1->decls.size(); i++){
      decls.push_back(std::move(decllist1->decls.at(i)));
    }
  }

  return make_unique<DeclListASTnode>(decls);
}

// decl_list1 -> decl decl_list1 | epsilon
unique_ptr<DeclListASTnode> parse_decl_list1(){
  vector<unique_ptr<DeclASTnode>> decls;
  unique_ptr<DeclListASTnode> decllist1;
  // vector<unique_ptr<ExternASTnode>> externlist1;
  unique_ptr<DeclASTnode> decl;
  // unique_ptr<DeclListASTnode> ans;

  sentence prod0 = rhslist[lhs_to_index("decl_list1")][0];
  sentence prod1_eps = rhslist[lhs_to_index("extern_list1")][1]; // this is epsilon
  

  if (in_sentence_first(CurTok, prod0)){
    decl = parse_decl();
    decls.push_back(std::move(decl));
    decllist1 = parse_decl_list1();
    for (int i=0; i<decllist1->decls.size(); i++){
      decls.push_back(std::move(decllist1->decls.at(i)));
    }
    return make_unique<DeclListASTnode>(decls);
  } else {
    return nullptr;
  }
}

// decl -> var_decl | fun_decl
unique_ptr<DeclASTnode> parse_decl(){
  sentence prod0 = rhslist[lhs_to_index("decl")][0];
  sentence prod1 = rhslist[lhs_to_index("decl")][1];

  TOKEN tempcur = CurTok;
  if ( !in_sentence_first(tempcur, prod0) && !in_sentence_first(tempcur, prod1)){
    throw ParseError(CurTok, "could not find production for decl");
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
    unique_ptr<FunDeclASTnode> fundecl = parse_fun_decl(); // not nullable
    return make_unique<DeclASTnode>(std::move(fundecl));
  } else if (temp2.type == SC){
    putBackToken(temp2);
    putBackToken(temp1);
    putBackToken(tempcur);
    getNextToken(); // CurTok is now back to what it was (same value as tempcur)
    unique_ptr<VarDeclASTnode> vardecl = parse_var_decl(); // not nullable
    return make_unique<DeclASTnode>(std::move(vardecl));
  } else {
    if (tempcur.type == VOID_TOK){
      throw ParseError(temp2, "was expecting '(' but got " + temp2.lexeme);
    } else {
      throw ParseError(temp2, "was expecting '(' or ';' but got " + temp2.lexeme);
    }
  }
}

// var_decl -> var_type IDENT ';'
unique_ptr<VarDeclASTnode> parse_var_decl(){
  sentence prod0 = rhslist[lhs_to_index("var_decl")][0];
  unique_ptr<VarTypeASTnode> vt;
  unique_ptr<IdentASTnode> ident;
  // if (in_sentence_first(CurTok, prod0)){
  //   unique_ptr<VarTypeASTnode> vt = parse_var_type();
  // } else {
  //   throw ParseError(CurTok, "could not find production for var_decl");
  // }
  vt = parse_var_type();
  if (CurTok.type != IDENT){
    throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  }
  ident = make_unique<IdentASTnode>(CurTok);
  getNextToken();
  if (CurTok.type != SC){
    throw ParseError(CurTok, "was expecting ';' but got " + CurTok.lexeme);
  }
  return make_unique<VarDeclASTnode>(std::move(vt), std::move(ident));
}

// type_spec -> 'void' | var_type
unique_ptr<TypeSpecASTnode> parse_type_spec(){
  // sentence prod0 = rhslist[lhs_to_index("type_spec")][0];
  sentence prod1 = rhslist[lhs_to_index("type_spec")][1];

  if (CurTok.type == VOID_TOK){
    getNextToken();
    return make_unique<TypeSpecASTnode>("void");
  } else if (in_sentence_first(CurTok, prod1)) {
    unique_ptr<VarTypeASTnode> vartypeptr = parse_var_type();
    return make_unique<TypeSpecASTnode>(std::move(vartypeptr));
  } else {
    throw ParseError(CurTok, "could not find production for type_spec");
  }
}

// var_type -> 'int' | 'float' | 'bool'
unique_ptr<VarTypeASTnode> parse_var_type(){
  if (CurTok.type == INT_TOK){
    getNextToken();
    return make_unique<VarTypeASTnode>("int");
  } else if (CurTok.type == FLOAT_TOK){
    getNextToken();
    return make_unique<VarTypeASTnode>("float");
  } else if (CurTok.type == BOOL_TOK){
    getNextToken();
    return make_unique<VarTypeASTnode>("bool");
  } else {
    throw ParseError(CurTok, "was expecting 'int', 'float' or 'bool' but got " + CurTok.lexeme);
  }
}

// fun_decl -> type_spec IDENT '(' params ')' block
unique_ptr<FunDeclASTnode> parse_fun_decl(){
  sentence prod0 = rhslist[lhs_to_index("fun_decl")][0];
  unique_ptr<TypeSpecASTnode> typespec;
  // string ident;
  unique_ptr<IdentASTnode> ident;
  
  unique_ptr<ParamListASTnode> params;
  unique_ptr<BlockASTnode> block;

  // 0
  // if (in_sentence_first(CurTok, prod0)){
  //   typespec = parse_type_spec(); // not nullable
  // } else {
  //   throw ParseError(CurTok, "could not find production for fun_decl");
  // }
  typespec = parse_type_spec(); // not nullable

  // 1
  if (CurTok.type == IDENT){
    // ident = CurTok.lexeme;
    ident = make_unique<IdentASTnode>(CurTok);
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

  return make_unique<FunDeclASTnode>(std::move(typespec), std::move(ident), std::move(params), std::move(block));
}

// params -> param_list | 'void' | epsilon
unique_ptr<ParamListASTnode> parse_params(){
  sentence prod0 = rhslist[lhs_to_index("params")][0];
  unique_ptr<ParamListASTnode> paramlist;
  if (in_sentence_first(CurTok, prod0)){
    paramlist = parse_param_list();
    return std::move(paramlist);
  } else if (CurTok.type == VOID_TOK) {
    getNextToken();
    return make_unique<ParamListASTnode>();
  } else {
    return make_unique<ParamListASTnode>();
  }
}

// param_list -> param param_list1
unique_ptr<ParamListASTnode> parse_param_list(){
  vector<unique_ptr<ParamASTnode>> paramlist;
  unique_ptr<ParamListASTnode> paramlist1;
  unique_ptr<ParamASTnode> param;

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
  //   return make_unique<ParamListASTnode>(paramlist);
  // } else {
  //   throw ParseError(CurTok, "could not find production for param_list");
  // }

  param = parse_param(); // not nullable
  paramlist.push_back(std::move(param));
  paramlist1 = parse_param_list1(); // is nullable
  if (paramlist1){
    for (int i=0; i<paramlist1->paramlist.size(); i++){
      paramlist.push_back(std::move(paramlist1->paramlist.at(i)));
    }
  }
  return make_unique<ParamListASTnode>(paramlist);
}

// param_list1 -> ',' param param_list1 | epsilon
unique_ptr<ParamListASTnode> parse_param_list1(){
  vector<unique_ptr<ParamASTnode>> paramlist;
  unique_ptr<ParamASTnode> param;
  unique_ptr<ParamListASTnode> paramlist1;
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
    return make_unique<ParamListASTnode>(paramlist);
  } else {
    return nullptr;
  }
}

// param -> var_type IDENT
unique_ptr<ParamASTnode> parse_param(){
  sentence prod0 = rhslist[lhs_to_index("param")][0];
  unique_ptr<VarTypeASTnode> vartype;
  // string ident;
  unique_ptr<IdentASTnode> ident;
  // if (in_sentence_first(CurTok, prod0)){
  //   vartype = parse_var_type();
  //   if (CurTok.type == IDENT){
  //     ident = CurTok.lexeme;
  //     getNextToken();
  //     return make_unique<ParamASTnode>(std::move(vartype), ident);
  //   } else {
  //     throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  //   }
  // } else {
  //   throw ParseError(CurTok, "could not find production for param");
  // }
  vartype = parse_var_type();
  if (CurTok.type == IDENT){
    // ident = CurTok.lexeme;
    ident = make_unique<IdentASTnode>(CurTok);
    getNextToken();
    return make_unique<ParamASTnode>(std::move(vartype), std::move(ident));
  } else {
      throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
    }
}

// block -> '{' local_decls stmt_list '}'
unique_ptr<BlockASTnode> parse_block(){
  unique_ptr<DeclListASTnode> localdecls;
  unique_ptr<StmtListASTnode> stmtlist;
  if (CurTok.type == LBRA){
    getNextToken();
    localdecls = parse_local_decls(); // is nullable
    stmtlist = parse_stmt_list();
    return make_unique<BlockASTnode>(std::move(localdecls), std::move(stmtlist));
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
unique_ptr<DeclListASTnode> parse_local_decls(){
  sentence prod0 = rhslist[lhs_to_index("local_decls")][0];
  vector<unique_ptr<DeclASTnode>> localdecllist;
  unique_ptr<DeclListASTnode> localdecllist1;
  unique_ptr<DeclASTnode> localdecl;

  if (in_sentence_first(CurTok, prod0)){
    localdecl = parse_local_decl();
    localdecllist.push_back(std::move(localdecl));
    localdecllist1 = parse_local_decls(); // is nullable
    if (localdecllist1){
      for (int i=0; i<localdecllist1->decls.size(); i++){
        localdecllist.push_back(std::move(localdecllist1->decls.at(i)));
      }
    }
    return make_unique<DeclListASTnode>(localdecllist);    
  } else {
    return nullptr;
  }
}

// local_decl -> var_type IDENT ';'
unique_ptr<DeclASTnode> parse_local_decl(){
  sentence prod0 = rhslist[lhs_to_index("local_decl")][0];
  unique_ptr<VarTypeASTnode> vartype;
  // string ident;
  unique_ptr<IdentASTnode> ident;
  vartype = parse_var_type();
  if (CurTok.type == IDENT){
    // ident = CurTok.lexeme;
    ident = make_unique<IdentASTnode>(CurTok);
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting IDENT but got " + CurTok.lexeme);
  }
  if (CurTok.type == SC){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting ';' but got " + CurTok.lexeme);
  }
  unique_ptr<VarDeclASTnode> vardecl = make_unique<VarDeclASTnode>(std::move(vartype), std::move(ident));

  return make_unique<DeclASTnode>(std::move(vardecl));
}

// stmt_list -> stmt stmt_list | epsilon
unique_ptr<StmtListASTnode> parse_stmt_list(){
  sentence prod0 = rhslist[lhs_to_index("stmt_list")][0];
  unique_ptr<StmtASTnode> stmt;
  vector<unique_ptr<StmtASTnode>> stmtlist;
  unique_ptr<StmtListASTnode> stmtlist1;
  if (in_sentence_first(CurTok, prod0)){
    stmt = parse_stmt();
    stmtlist.push_back(std::move(stmt));
    stmtlist1 = parse_stmt_list(); //is nullable
    if (stmtlist1){
      for (int i=0; i<stmtlist1->stmtlist.size(); i++){
        stmtlist.push_back(std::move(stmtlist1->stmtlist.at(i)));
      }
    }
    return make_unique<StmtListASTnode>(stmtlist);
  } else {
    return nullptr;
  }
}

// stmt -> expr_stmt | block | if_stmt | while_stmt | return_stmt
unique_ptr<StmtASTnode> parse_stmt(){
  sentence prod0 = rhslist[lhs_to_index("stmt")][0];
  sentence prod1 = rhslist[lhs_to_index("stmt")][1];
  sentence prod2 = rhslist[lhs_to_index("stmt")][2];
  sentence prod3 = rhslist[lhs_to_index("stmt")][3];
  sentence prod4 = rhslist[lhs_to_index("stmt")][4];
  if (in_sentence_first(CurTok, prod0)){
    unique_ptr<ExprASTnode> expr_stmt = parse_expr_stmt();
    return make_unique<StmtASTnode>(std::move(expr_stmt));
  } else if (in_sentence_first(CurTok, prod1)){
    unique_ptr<BlockASTnode> block = parse_block();
    return make_unique<StmtASTnode>(std::move(block));
  } else if (in_sentence_first(CurTok, prod2)){
    unique_ptr<IfASTnode> if_stmt = parse_if_stmt();
    return make_unique<StmtASTnode>(std::move(if_stmt));
  } else if (in_sentence_first(CurTok, prod3)){
    unique_ptr<WhileASTnode> while_stmt = parse_while_stmt();
    return make_unique<StmtASTnode>(std::move(while_stmt));
  } else if (in_sentence_first(CurTok, prod4)){
    unique_ptr<ReturnASTnode> return_stmt = parse_return_stmt();
    return make_unique<StmtASTnode>(std::move(return_stmt));
  } else {
    throw ParseError(CurTok, "could not find production for stmt");
  }
}

// expr_stmt -> expr ';' | ';'
unique_ptr<ExprASTnode> parse_expr_stmt(){
  sentence prod0 = rhslist[lhs_to_index("expr_stmt")][0];
  unique_ptr<ExprASTnode> expr;
  if (in_sentence_first(CurTok, prod0)){
    // std::cout << "Before parse_expr CurTok is " << CurTok.lexeme << "\n";
    expr = parse_expr();
    // std::cout << "After parse_expr CurTok is " << CurTok.lexeme << "\n";
    if (CurTok.type == SC){
      getNextToken();
    } else {
      throw ParseError(CurTok, "was expecting ';' but got " + CurTok.lexeme);
    }
    return std::move(expr);
  } else if (CurTok.type == ';'){
    getNextToken();
    // return make_unique<ExprASTnode>();
    return nullptr;
  } else {
    throw ParseError(CurTok, "could not find production for expr");
  }
}

// while_stmt -> 'while' '(' expr ')' stmt
unique_ptr<WhileASTnode> parse_while_stmt(){
  // 0
  if (CurTok.type == WHILE){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting 'while' but got " + CurTok.lexeme);
  }

  // 1
  if (CurTok.type == LPAR){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting '(' but got " + CurTok.lexeme);
  }

  // 2
  unique_ptr<ExprASTnode> expr = parse_expr();

  // 3
  if (CurTok.type == RPAR){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting ')' but got " + CurTok.lexeme);
  }

  // 4
  unique_ptr<StmtASTnode> stmt = parse_stmt();

  return make_unique<WhileASTnode>(std::move(expr), std::move(stmt));
}

// if_stmt -> 'if' '(' expr ')' block else_stmt
unique_ptr<IfASTnode> parse_if_stmt(){
  // 0
  if (CurTok.type == IF){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting 'if' but got " + CurTok.lexeme);
  }

  // 1
  if (CurTok.type == LPAR){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting '(' but got " + CurTok.lexeme);
  }

  // 2
  unique_ptr<ExprASTnode> expr = parse_expr();

  // 3
  if (CurTok.type == RPAR){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting ')' but got " + CurTok.lexeme);
  }

  // 4
  unique_ptr<BlockASTnode> block = parse_block();

  // 5
  unique_ptr<BlockASTnode> else_stmt = parse_else_stmt();

  return make_unique<IfASTnode>(std::move(expr), std::move(block), std::move(else_stmt));  
}

// else_stmt -> 'else' block | epsilon
unique_ptr<BlockASTnode> parse_else_stmt(){
  if (CurTok.type == ELSE){
    getNextToken();
    unique_ptr<BlockASTnode> block = parse_block();
    return std::move(block);
  } else {
    return nullptr;
  }
}

// return_stmt -> 'return' ';' | 'return' expr ';'
unique_ptr<ReturnASTnode> parse_return_stmt(){
  TOKEN tempcur = CurTok;
  if (CurTok.type == RETURN){
    getNextToken();
  } else {
    throw ParseError(CurTok, "was expecting 'return' but got " + CurTok.lexeme);
  }
  
  TOKEN temp1 = CurTok;
  if (temp1.type == SC){
    getNextToken();
    return make_unique<ReturnASTnode>();
  } else {
    unique_ptr<ExprASTnode> expr = parse_expr();
    if (CurTok.type == SC){
      getNextToken();
    } else {
      throw ParseError(CurTok, "was expecting ';' but got " + CurTok.lexeme);
    }
    return make_unique<ReturnASTnode>(std::move(expr));
  }
}

// expr -> IDENT '=' expr | rval
unique_ptr<ExprASTnode> parse_expr(){
  // if we parse the first production only if current token
  // is IDENT and the next token is '='
  sentence prod1 = rhslist[lhs_to_index("expr")][1];
  //unique_ptr<ExprASTnode> ans;
  unique_ptr<ExprASTnode> expr;
  unique_ptr<IdentASTnode> ident;
  if (CurTok.type == IDENT){
    TOKEN tempcur = CurTok;
    ident = make_unique<IdentASTnode>(CurTok);
    getNextToken();
    TOKEN temp1 = CurTok;
    if (temp1.type == ASSIGN){
      getNextToken();
      expr = parse_expr();
      //ans = make_unique<ExprASTnode>();
      unique_ptr<AssignASTnode> assignast = make_unique<AssignASTnode>(std::move(ident), std::move(expr));
      
      return make_unique<ExprASTnode>(std::move(assignast));
    } else {
      putBackToken(temp1);
      putBackToken(tempcur);
      getNextToken(); // CurTok now has the same value as tempcur
      expr = parse_rval();
      //ans = make_unique<ExprASTnode>();
      return std::move(expr);
    }
  } else if (in_sentence_first(CurTok, prod1)){
    expr = parse_rval();
    return std::move(expr);
  } else {
    throw ParseError(CurTok, "could not find production for expr");
  }
}





// // // rval -> rval1 '||' rval | rval1
// // unique_ptr<ExprASTnode> parse_rval(){
// //   sentence prod0 = rhslist[lhs_to_index("rval")][0]; // this is not nullable
// //   // note the first set of sentence prod0 is: LPAR, MINUS, NOT, BOOL_LIT, INT_LIT, FLOAT_LIT
// //   if (in_sentence_first(CurTok, prod0)) {
// //     if (CurTok == LPAR || CurTok == MINUS)
// //     // if LPAR, MINUS, NOT then do second production
// //     // otherwise look ahead 1 step. if OR then do first production. otherwise do second production.
// //   } else {
// //     throw ParseError(CurTok, "could not find production for rval");
// //   }
// // } 

// rval -> rval1 rval_lf
unique_ptr<ExprASTnode> parse_rval(){
  auto expr = parse_rval1();
  auto binop = parse_rval_lf();
  if (binop) {
    binop->lhs = std::move(expr);
    return make_unique<ExprASTnode>(std::move(binop));
  } else {
    return std::move(expr);
  }
}

// rval_lf -> '||' rval | epsilon
unique_ptr<BinOpASTnode> parse_rval_lf(){
  if (CurTok.type == OR) {
    getNextToken();
    auto rhs = parse_rval();
    return make_unique<BinOpASTnode>(OR, nullptr, std::move(rhs));
  } else {
    return nullptr;
  }
}

// rval1 -> rval2 rval1_lf
unique_ptr<ExprASTnode> parse_rval1(){
  auto expr = parse_rval2();
  auto binop = parse_rval1_lf();
  if (binop) {
    binop->lhs = std::move(expr);
    return make_unique<ExprASTnode>(std::move(binop));
  } else {
    return std::move(expr);
  }
}

// rval1_lf -> '&&' rval1 | epsilon
unique_ptr<BinOpASTnode> parse_rval1_lf(){
  if (CurTok.type == AND) {
    getNextToken();
    auto rhs = parse_rval1();
    return make_unique<BinOpASTnode>(AND, nullptr, std::move(rhs));
  } else {
    return nullptr;
  }
}

// rval2 -> rval3 rval2_lf
unique_ptr<ExprASTnode> parse_rval2(){
  auto expr = parse_rval3();
  auto binop = parse_rval2_lf();
  if (binop) {
    binop->lhs = std::move(expr);
    return make_unique<ExprASTnode>(std::move(binop));
  } else {
    return std::move(expr);
  }
}

// rval2_lf -> '==' rval2 | '!=' rval2 | epsilon
unique_ptr<BinOpASTnode> parse_rval2_lf(){
  if (CurTok.type == EQ) {
    getNextToken();
    auto rhs = parse_rval2();
    return make_unique<BinOpASTnode>(EQ, nullptr, std::move(rhs));
  } else if (CurTok.type == NE) {
    getNextToken();
    auto rhs = parse_rval2();
    return make_unique<BinOpASTnode>(NE, nullptr, std::move(rhs));
  } else {
    return nullptr;
  }
}

// rval3 -> rval4 rval3_lf
unique_ptr<ExprASTnode> parse_rval3(){
  auto expr = parse_rval4();
  auto binop = parse_rval3_lf();
  if (binop) {
    binop->lhs = std::move(expr);
    return make_unique<ExprASTnode>(std::move(binop));
  } else {
    return std::move(expr);
  }
}

// rval3_lf -> '<=' rval3 | '<' rval3 | '>=' rval3 | '>' rval3 | epsilon
unique_ptr<BinOpASTnode> parse_rval3_lf(){
  if (CurTok.type == LE) {
    getNextToken();
    auto rhs = parse_rval3();
    return make_unique<BinOpASTnode>(LE, nullptr, std::move(rhs));
  } else if (CurTok.type == LT) {
    getNextToken();
    auto rhs = parse_rval3();
    return make_unique<BinOpASTnode>(LT, nullptr, std::move(rhs));
  } else if (CurTok.type == GE) {
    getNextToken();
    auto rhs = parse_rval3();
    return make_unique<BinOpASTnode>(GE, nullptr, std::move(rhs));
  } else if (CurTok.type == GT) {
    getNextToken();
    auto rhs = parse_rval3();
    return make_unique<BinOpASTnode>(GT, nullptr, std::move(rhs));
  } else {
    return nullptr;
  }
}

// rval4 -> rval5 rval4_lf
unique_ptr<ExprASTnode> parse_rval4(){
  auto expr = parse_rval5();
  auto binop = parse_rval4_lf();
  if (binop) {
    binop->lhs = std::move(expr);
    return make_unique<ExprASTnode>(std::move(binop));
  } else {
    return std::move(expr);
  }
}

// rval4_lf -> '+' rval4 | '-' rval4 | epsilon
unique_ptr<BinOpASTnode> parse_rval4_lf(){
  if (CurTok.type == PLUS) {
    getNextToken();
    auto rhs = parse_rval4();
    return make_unique<BinOpASTnode>(PLUS, nullptr, std::move(rhs));
  } else if (CurTok.type == MINUS) {
    getNextToken();
    auto rhs = parse_rval4();
    return make_unique<BinOpASTnode>(MINUS, nullptr, std::move(rhs));
  } else {
    return nullptr;
  }
}

// rval5 -> rval6 rval5_lf
unique_ptr<ExprASTnode> parse_rval5(){
  auto expr = parse_rval6();
  auto binop = parse_rval5_lf();
  if (binop) {
    binop->lhs = std::move(expr);
    return make_unique<ExprASTnode>(std::move(binop));
  } else {
    return std::move(expr);
  }
}

// rval5_lf -> '*' rval5 | '/' rval5 | '%' rval5 | epsilon
unique_ptr<BinOpASTnode> parse_rval5_lf(){
  if (CurTok.type == ASTERIX) {
    getNextToken();
    auto rhs = parse_rval5();
    return make_unique<BinOpASTnode>(ASTERIX, nullptr, std::move(rhs));
  } else if (CurTok.type == DIV) {
    getNextToken();
    auto rhs = parse_rval5();
    return make_unique<BinOpASTnode>(DIV, nullptr, std::move(rhs));
  } else if (CurTok.type == MOD) {
    getNextToken();
    auto rhs = parse_rval5();
    return make_unique<BinOpASTnode>(MOD, nullptr, std::move(rhs));
  } else {
    return nullptr;
  }
}

// rval6 -> '-' rval6 | '!' rval6 | rval7
unique_ptr<ExprASTnode> parse_rval6(){
  sentence prod2 = rhslist[lhs_to_index("rval6")][2];
  if (CurTok.type == MINUS){
    getNextToken();
    auto expr = parse_rval6();
    auto unop = make_unique<UnOpASTnode>(MINUS, std::move(expr));
    return make_unique<ExprASTnode>(std::move(unop));
  } else if (CurTok.type == NOT) {
    getNextToken();
    auto expr = parse_rval6();
    auto unop = make_unique<UnOpASTnode>(NOT, std::move(expr));
    return make_unique<ExprASTnode>(std::move(unop));
  } else if (in_sentence_first(CurTok, prod2)){
    auto expr = parse_rval7();
    return std::move(expr);
  } else {
    throw ParseError(CurTok, "could not find production for rval6");
  }
}

// rval7 -> '(' expr ')' | IDENT | IDENT '(' args ')' | INT_LIT | FLOAT_LIT | BOOL_LIT
unique_ptr<ExprASTnode> parse_rval7(){
  // '(' expr ')'
  // std::cout << "in rval7\n";
  if (CurTok.type == LPAR){
    getNextToken();
    unique_ptr<ExprASTnode> expr = parse_expr();
    if (CurTok.type == RPAR){
      getNextToken();
    } else {
      throw ParseError(CurTok, "was expecting ')' but got " + CurTok.lexeme);
    }
    return std::move(expr);
  } else if (CurTok.type == INT_LIT) { // INT_LIT
    auto intlit = make_unique<IntASTnode>(CurTok);
    getNextToken();
    return make_unique<ExprASTnode>(std::move(intlit));
  } else if (CurTok.type == FLOAT_LIT) { // FLOAT_LIT
    auto floatlit = make_unique<FloatASTnode>(CurTok);
    getNextToken();
    return make_unique<ExprASTnode>(std::move(floatlit));
  } else if (CurTok.type == BOOL_LIT) { // INT_LIT
    auto boollit = make_unique<BoolASTnode>(CurTok);
    getNextToken();
    return make_unique<ExprASTnode>(std::move(boollit));
  } else if (CurTok.type == IDENT){ // IDENT and IDENT '(' args ')'
    auto ident = make_unique<IdentASTnode>(CurTok);
    getNextToken();
    if (CurTok.type != LPAR) {
      return make_unique<ExprASTnode>(std::move(ident));
    } else {
      getNextToken();
      auto args = parse_args();
      if (CurTok.type == RPAR){
        getNextToken();
      } else {
        throw ParseError(CurTok, "was expecting ')' but got " + CurTok.lexeme);
      }
      auto fc = make_unique<FunCallASTnode>(std::move(ident), std::move(args));
      return make_unique<ExprASTnode>(std::move(fc));
    }
  } else {
    throw ParseError(CurTok, "could not find production for rval7");
  }
}

// args -> arg_list | epsilon
unique_ptr<ArgListASTnode> parse_args(){
  sentence prod0 = rhslist[lhs_to_index("args")][0];
  unique_ptr<ArgListASTnode> arglist;
  if (in_sentence_first(CurTok, prod0)){
    arglist = parse_arg_list();
    return std::move(arglist);
  } else {
    return nullptr;
  }
}

// arg_list -> expr arg_list1
unique_ptr<ArgListASTnode> parse_arg_list(){
  unique_ptr<ExprASTnode> expr;
  vector<unique_ptr<ExprASTnode>> arglist;
  unique_ptr<ArgListASTnode> arglist1;
  expr = parse_expr();
  arglist.push_back(std::move(expr));
  arglist1 = parse_arg_list1(); // is nullable
  if (arglist1){
    for (int i=0; i<arglist1->arglist.size(); i++){
      arglist.push_back(std::move(arglist1->arglist.at(i)));
    }
  }
  return make_unique<ArgListASTnode>(arglist);
}

// arg_list1 -> ',' expr arg_list1 | epsilon
unique_ptr<ArgListASTnode> parse_arg_list1(){
  unique_ptr<ExprASTnode> expr;
  vector<unique_ptr<ExprASTnode>> arglist;
  unique_ptr<ArgListASTnode> arglist1;
  if (CurTok.type == COMMA){
    getNextToken();
    expr = parse_expr();
    arglist.push_back(std::move(expr));
    arglist1 = parse_arg_list1(); // is nullable
    if (arglist1){
      for (int i=0; i<arglist1->arglist.size(); i++){
        arglist.push_back(std::move(arglist1->arglist.at(i)));
      }
    }
    return make_unique<ArgListASTnode>(arglist);
  } else {
    return nullptr;
  }
}