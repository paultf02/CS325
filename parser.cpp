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

// start -> program EOF
// parse_start(){}
// program -> extern_list decl_list | decl_list
std::unique_ptr<ProgramASTnode> parse_program(){
  std::vector<std::unique_ptr<ExternASTnode>> externs; // does this default to empty vector
  std::vector<std::unique_ptr<DeclASTnode>> decls;
  std::unique_ptr<ProgramASTnode> ans;
  ans = std::make_unique<ProgramASTnode>();

  // if (in_sentence_first(sentence)){
  //   externs = parse_extern_list();
  //   decls = parse_decl_list();
  //   ans = std::make_unique<ProgramASTnode>(externs, decls);
  // } else {
  //   auto decls = parse_decl_list();
  //   ans = std::make_unique<ProgramASTnode>(decls);
  // }


  return std::move(ans);
}
// // extern_list -> extern extern_list1
// std::vector<std::unique_ptr<ExternASTnode>> parse_extern_list(){}
// // extern_list1 -> extern extern_list1 | epsilon
// parse_extern_list1(){}
// // extern -> 'extern' type_spec IDENT '(' params ')' ';'
// parse_extern(){}
// // decl_list -> decl decl_list1
// std::vector<std::unique_ptr<DeclASTnode>> parse_decl_list(){}
// // decl_list1 -> decl decl_list1 | epsilon
// parse_decl_list1(){}
// // decl -> var_decl | fun_decl
// parse_decl(){}
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

// int word_to_type(std::string word){
//   int type = -100;
//   if (word=="IDENT"){
//     type = -1;
//   } else if (word=="'='"){
//     type = int('=');
//   } else if (word=="'{'"){
//     type = int('{');
//   } else if (word=="'}'"){
//     type = int('}');
//   } else if (word=="'('"){
//     type = int('(');
//   } else if (word=="')'"){
//     type = int(')');
//   } else if (word=="';'"){
//     type = int(';');
//   } else if (word=="','"){
//     type = int(',');
//   } else if (word=="'int'"){
//     type = -2;
//   } else if (word=="'void'"){
//     type = -3;
//   } else if (word=="'float'"){
//     type = -4;
//   } else if (word=="'bool'"){
//     type = -5;
//   } else if (word=="'extern'"){
//     type = -6;
//   } else if (word=="'if'"){
//     type = -7;
//   } else if (word=="'else'"){
//     type = -8;
//   } else if (word=="'while'"){
//     type = -9;
//   } else if (word=="'return'"){
//     type = -10;
//   } else if (word=="INT_LIT"){
//     type = -14;
//   } else if (word=="FLOAT_LIT"){
//     type = -15;
//   } else if (word=="BOOL_LIT"){
//     type = -16;
//   } else if (word=="'&&'"){
//     type = -17;
//   } else if (word=="'||'"){
//     type = -18;
//   } else if (word=="'+'"){
//     type = int('+');
//   } else if (word=="'-'"){
//     type = int('-');
//   } else if (word=="'*'"){
//     type = int('*');
//   } else if (word=="'/'"){
//     type = int('/');
//   } else if (word=="'%'"){
//     type = int('%');
//   } else if (word=="'!'"){
//     type = int('!');
//   } else if (word=="'=='"){
//     type = -19;
//   } else if (word=="'!='"){
//     type = -20;
//   } else if (word=="'<='"){
//     type = -21;
//   } else if (word=="'<'"){
//     type = int('<');
//   } else if (word=="'>='"){
//     type = -23;
//   } else if (word=="'>'"){
//     type = int('>');
//   } else if (word=="EOF"){
//     type = 0;
//   } else {
//     type = -100;
//   }
//   return type;
// }

// std::vector<int> terminals_to_int(std::vector<std::string> list_of_terminals){
//   std::vector<int> intvector(list_of_terminals.size());
//   for (int i=0; i<list_of_terminals.size();i++){
//     intvector[i] = word_to_type(list_of_terminals[i]);
//   }
//   return intvector;
// }