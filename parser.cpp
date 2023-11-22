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

// do you need to fill the below in so that we can eat the EOF?
// start -> program EOF
// parse_start(){}

// program -> extern_list decl_list | decl_list
std::unique_ptr<ProgramASTnode> parse_program(){
  // std::vector<std::unique_ptr<ExternASTnode>> externs; // these default to empty vector
  // std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::unique_ptr<ExternListASTnode> externlist = nullptr;
  std::unique_ptr<DeclListASTnode> decllist = nullptr;
  std::unique_ptr<ProgramASTnode> ans;
  // ans = std::make_unique<ProgramASTnode>();

  sentence prod0 = rhslist[lhs_to_index("program")][0];
  sentence prod1 = rhslist[lhs_to_index("program")][1];

  if (in_sentence_first(CurTok, prod0)){
    // program -> extern_list decl_list
    externlist = parse_extern_list();
    // decls = parse_decl_list();
    // ans = std::make_unique<ProgramASTnode>(std::move(externs), std::move(decls));
    // ans = std::make_unique<ProgramASTnode>(externs, decls);
    // ans = std::make_unique<ProgramASTnode>();
    ans = std::make_unique<ProgramASTnode>(std::move(externlist), std::move(decllist));

  } else if (in_sentence_first(CurTok, prod1)){
    // program -> decl_list
    decllist = parse_decl_list();
    // ans = std::make_unique<ProgramASTnode>(std::move(externs), std::move(decls));
    // ans = std::make_unique<ProgramASTnode>(externs, decls);
    // ans = std::make_unique<ProgramASTnode>();
    ans = std::make_unique<ProgramASTnode>(std::move(externlist), std::move(decllist));
  } else {
    throw ParseError(CurTok, "could not parse program");
  }

  // return std::move(ans);
  return ans;
}
// extern_list -> extern extern_list1
std::unique_ptr<ExternListASTnode> parse_extern_list(){
  std::vector<std::unique_ptr<ExternASTnode>> externs;
  std::unique_ptr<ExternListASTnode> externlist1;
  // std::vector<std::unique_ptr<ExternASTnode>> externlist1;
  std::unique_ptr<ExternASTnode> ext;
  std::unique_ptr<ExternListASTnode> ans;

  sentence prod0 = rhslist[lhs_to_index("extern_list")][0];

  if (in_sentence_first(CurTok, prod0)){
    ext = parse_extern();
    externs.push_back(std::move(ext));
    externlist1 = parse_extern_list1();
    for (int i=0; i<externlist1->externs.size(); i++){
      externs.push_back(std::move(externlist1->externs.at(i)));
    }
    ans = std::make_unique<ExternListASTnode>(externs);
  } else {
    throw ParseError(CurTok, "could not parse extern_list");
  }
  return ans;
}

// extern_list1 -> extern extern_list1 | epsilon
std::unique_ptr<ExternListASTnode> parse_extern_list1(){
  return nullptr;
}

// extern -> 'extern' type_spec IDENT '(' params ')' ';'
std::unique_ptr<ExternASTnode> parse_extern(){
  return nullptr;
}

// decl_list -> decl decl_list1
std::unique_ptr<DeclListASTnode> parse_decl_list(){
  std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::unique_ptr<DeclListASTnode> decllist1;
  // std::vector<std::unique_ptr<ExternASTnode>> externlist1;
  std::unique_ptr<DeclASTnode> decl;
  std::unique_ptr<DeclListASTnode> ans;

  sentence prod0 = rhslist[lhs_to_index("decl_list")][0];

  if (in_sentence_first(CurTok, prod0)){
    decl = parse_decl();
    decls.push_back(std::move(decl));
    decllist1 = parse_decl_list1();
    for (int i=0; i<decllist1->decls.size(); i++){
      decls.push_back(std::move(decllist1->decls.at(i)));
    }
    ans = std::make_unique<DeclListASTnode>(decls);
  } else {
    throw ParseError(CurTok, "could not parse decl_list");
  }
  return ans;
}

// decl_list1 -> decl decl_list1 | epsilon
std::unique_ptr<DeclListASTnode> parse_decl_list1(){
  return nullptr;
}
// decl -> var_decl | fun_decl
std::unique_ptr<DeclASTnode> parse_decl(){
  return nullptr;
}
// // var_decl -> var_type IDENT ';'
// parse_var_decl(){}
// // type_spec -> 'void' | var_type
// parse_type_spec(){}
// // var_type -> 'int' | 'float' | 'bool'
// parse_var_type(){}
// // fun_decl -> type_spec IDENT '(' params ')' block
// parse_fun_decl(){}
// // params -> param_list | 'void' | epsilon
// parse_params(){}
// // param_list -> param param_list1
// parse_param_list(){}
// // param_list1 -> ',' param param_list1 | epsilon
// parse_param_list1(){}
// // param -> var_type IDENT
// parse_param(){}
// // block -> '{' local_decls stmt_list '}'
// parse_block(){}
// // local_decls -> local_decl local_decls | epsilon
// parse_local_decls(){}
// // local_decl -> var_type IDENT ';'
// parse_local_decl(){}
// // stmt_list -> stmt stmt_list | epsilon
// parse_stmt_list(){}
// // stmt -> expr_stmt | block | if_stmt | while_stmt | return_stmt
// parse_stmt(){}
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



