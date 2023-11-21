/*
Recursive descent predictive parsing
void A(){
  choose A-production A -> X1X2...Xk
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

// start -> program EOF
parse_start(){}
// program -> extern_list decl_list | decl_list
parse_program(){}
// extern_list -> extern extern_list1
parse_extern_list(){}
// extern_list1 -> extern extern_list1 | epsilon
parse_extern_list1(){}
// extern -> 'extern' type_spec IDENT '(' params ')' ';'
parse_extern(){}
// decl_list -> decl decl_list1
parse_decl_list(){}
// decl_list1 -> decl decl_list1 | epsilon
parse_decl_list1(){}
// decl -> var_decl | fun_decl
parse_decl(){}
// var_decl -> var_type IDENT ';'
parse_var_decl(){}
// type_spec -> 'void' | var_type
parse_type_spec(){}
// var_type -> 'int' | 'float' | 'bool'
parse_var_type(){}
// fun_decl -> type_spec IDENT '(' params ')' block
parse_fun_decl(){}
// params -> param_list | 'void' | epsilon
parse_params(){}
// param_list -> param param_list1
parse_param_list(){}
// param_list1 -> ',' param param_list1 | epsilon
parse_param_list1(){}
// param -> var_type IDENT
parse_param(){}
// block -> '{' local_decls stmt_list '}'
parse_block(){}
// local_decls -> local_decl local_decls | epsilon
parse_local_decls(){}
// local_decl -> var_type IDENT ';'
parse_local_decl(){}
// stmt_list -> stmt stmt_list | epsilon
parse_stmt_list(){}
// stmt -> expr_stmt | block | if_stmt | while_stmt | return_stmt
parse_stmt(){}
// expr_stmt -> expr ';' | ';'
parse_expr_stmt(){}
// while_stmt -> 'while' '(' expr ')' stmt
parse_while_stmt(){}
// if_stmt -> 'if' '(' expr ')' block else_stmt
parse_if_stmt(){}
// else_stmt -> 'else' block | epsilon
parse_else_stmt(){}
// return_stmt -> 'return' ';' | 'return' expr ';'
parse_return_stmt(){}
// expr -> IDENT '=' expr | rval
parse_expr(){}
// rval -> rval1 '||' rval | rval1
parse_rval(){}
// rval1 -> rval2 '&&' rval1 | rval2
parse_rval1(){}
// rval2 -> rval3 '==' rval2 | rval3 '!=' rval2 | rval3
parse_rval2(){}
// rval3 -> rval4 '<=' rval3 | rval4 '<' rval3 | rval4 '>=' rval3 | rval4 '>' rval3 | rval4
parse_rval3(){}
// rval4 -> rval5 '+' rval4 | rval5 '-' rval4 | rval5
parse_rval4(){}
// rval5 -> rval6 '*' rval5 | rval6 '/' rval5 | rval6 '%' rval5 | rval6
parse_rval5(){}
// rval6 -> '-' rval6 | '!' rval6 | rval7
parse_rval6(){}
// rval7 -> '(' expr ')' | IDENT | IDENT '(' args ')' | INT_LIT | FLOAT_LIT | BOOL_LIT
parse_rval7(){}
// args -> arg_list | epsilon
parse_args(){}
// arg_list -> expr arg_list1
parse_args_list(){}
// arg_list1 -> ',' expr arg_list1 | epsilon
parse_args_list1(){}