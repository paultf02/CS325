/*
void production_call(std::string name){
  sentence s = choose_production(name);
  for (int i = 0; i<s.length; i++){
    std::string element = s[i];
    if (element in nonterminals){
      production_call(element);
    } else if (element == CurTok){
      // create an AST node and add it to the tree
      CurTok++
    } else{
      //raise error
    }
  }
}
*/

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

std::vector<int> terminals_to_int(std::vector<std::string> terminals){
  std::vector<int> intvector(terminals.size());
  for (int i=0; i<terminals.size();i++){
    intvector[i] = word_to_type(terminals[i]);
  }
  return intvector;
}

bool sentencenullable(sentence &sentence){
  bool ans = true;
  for (auto &word: sentence){
    if (word == "epsilon"){
      continue;
    }
    else if (std::find(terminals.begin(), terminals.end(), word) != terminals.end()){
      return false;
    } else {
        ans = ans && nullable[word];
    }
  }
  return ans;
}

sentence look_ahead_special_case(std::string nonterminal, production_options productions){
  sentence answer;
  if (nonterminal == "decl"){
    // what if we cant index this far
    if (curTokIndex + 2 >= program_tokens.size()){
      throw std::runtime_error("missing some tokens");
    } else if (program_tokens[curTokIndex+2].type == word_to_type("'('")){
      answer = productions[1];
    } else {
      //this should have a token ';' at program_tokens[curTokIndex+2]
      answer = productions[0];
    }
  } else if (nonterminal == "return_stmt"){
      if (curTokIndex + 1 >= program_tokens.size()){
        throw std::runtime_error("missing some tokens");
      } else if (program_tokens[curTokIndex+1].type == word_to_type("';'")){
        answer = productions[0];
      } else {
        answer = productions[1];
      }
  } else if (nonterminal == "expr"){
      if (curTokIndex + 1 >= program_tokens.size()){
        throw std::runtime_error("missing some tokens");
      } else if (program_tokens[curTokIndex+1].type == word_to_type("'='")){
        answer = productions[0];
      } else {
        answer = productions[1];
      }
  } else if (nonterminal == "rval7"){
      if (curTokIndex + 1 >= program_tokens.size()){
        throw std::runtime_error("missing some tokens");
      } else if (program_tokens[curTokIndex+1].type == word_to_type("'('")){
        answer = productions[2];
      } else {
        answer = productions[1];
      }
  }
  return answer;
}

sentence choose_production(std::string nonterminal, production_options productions){
  /*
  let productions = s1, ..., sk 
  so si is a sentence
  if curTok is in exactly one of first(s1), ... ,first(sk) namely si then
    return si
  else if curTok in multiple of them then
    use furtheer lookahead to uniquely determine which si to return
  else if curTok is in zero of them then
    if curTok in follow(nonterminal) then
      if exactly one of the s1, ..., sk is nullable namely si then
        return si
      if many of s1, ..., sk are nullable then
        is this an ambiguous grammar??
        we believe in our grammar that at most one option is nullable
        raise error
      else if none of s1, ..., sk are nullable then
        raise error
    else
      raise error
  */
  sentence answer;
  int numfirstsets = 0;
  int lastsetindex = 0;
  for (int i=0; i<productions.size(); i++){
    auto prod = productions[i];
    if (prod[0] == "epsilon"){
      continue;
    }
    std::vector<std::string> sentencefirstold = find_sentence_first(prod);

    if (nonterminal == "expr"){
      std::cout << "elements of first set of expr production i=" << i << '\n';
      for (auto &elem : sentencefirstold){
        std::cout << elem << '\n';
      }
    }

    std::vector<int> sentencefirst = terminals_to_int(sentencefirstold);
    if (nonterminal == "expr"){
      std::cout << "elements of first set of expr production i=" << i << '\n';
      for (auto &elem : sentencefirst){
        std::cout << elem << '\n';
      }
    }

    if (std::find(sentencefirst.begin(), sentencefirst.end(), program_tokens[curTokIndex].type) != sentencefirst.end()){
      numfirstsets += 1;
      lastsetindex = i;
    }
  }

  if (numfirstsets == 1){
      answer =  productions[lastsetindex];
  } else if (numfirstsets > 1){
      std::cout << "we will use lookahead\n";
      answer = look_ahead_special_case(nonterminal, productions); // this needs to handle the special case where we lookahead
  } else if (numfirstsets == 0){
      std::vector<int> nontermfollowset = terminals_to_int(follow[nonterminal]);
      if (std::find(nontermfollowset.begin(), nontermfollowset.end(), program_tokens[curTokIndex].type) != nontermfollowset.end()){
        // current token is in follow[nonterminal]
        int numnullable = 0;
        int lastnullableindex = 0;
        for (int i; i<productions.size(); i++){
          sentence s = productions[i];
          if (sentencenullable(s)){
            numnullable +=1;
            lastnullableindex = i;
          }
        }
        if (numnullable == 1){
          answer = productions[lastnullableindex];
        }
        else if (numnullable > 1){
          throw std::runtime_error("There is an issue with the grammar because more than one set is nullable");
        }
        else if (numnullable == 0){
          throw std::runtime_error("there is a syntax error, no production is nullable");
        }
      }
  } else {
    throw std::runtime_error("error");
  }
  if (answer.size() == 0){
    std::string err_msg = "nonterminal: " + nonterminal + " current token: " + program_tokens[curTokIndex].lexeme;
    throw std::runtime_error("we have not been able to find a production and this is not a case we have seen\n" + err_msg);
  }
  return answer;
}

/*
Templated or conditional code

parse_general(node){
  returns pointer to node
  (inside the node there are children)
}

programpointer = parse_general("start")

*/

// create a parse tree
class ParseTreeNode{
  std::string name;
  std::vector<std::unique_ptr<ParseTreeNode>> children;
}

// now turn the parse tree into an AST
// extern_list and children that are extern and extern_list1 can be turned into one node
// same for decl_list, param_list, local_decls, stmt_list, arg_list
// expr mixes assignment and nested expressions.
// we want to remove rvals and then make it all 
// take a particular look at nodes with keywords and then turn them into an AST node without keyword children
// so for if_stmt then turn it into IfThenElseASTnode



void parse_general(std::string nonterminal, int depth){
  std::cout << nonterminal << " depth=" << depth << '\n';
  if (nonterminal=="epsilon"){
    return;
  }
  production_options productions = rhslist[nonterminal_index(nonterminal)];
  // std::cout << "index: " << nonterminal_index(nonterminal) << '\n';
  //note it will only have one
  // find some way of choosing this
  sentence prod = choose_production(nonterminal, productions);
  // std::cout << "finished choosing production\n";
  for (int i=0; i<prod.size(); i++){
    if (std::find(terminals.begin(), terminals.end(), prod[i]) == terminals.end()){
      // prod[i] is not a terminal 
      parse_general(prod[i], depth+1);
    } else if (word_to_type(prod[i]) == program_tokens[curTokIndex].type){
      // prod[i] is a terminal and matches the current token
      curTokIndex++;
      std::cout << "next token which is " << program_tokens[curTokIndex].lexeme << " lineno="<< program_tokens[curTokIndex].lineNo <<" type=" << program_tokens[curTokIndex].type <<" depth=" << depth << '\n';
      if (program_tokens[curTokIndex].type == 0){
        return;
      }
      // curNode.children = something;
    } else {
      // prod[i] is a terminal but doesn't match current token
      std::string err_msg = "compiling error with token number " + std::to_string(curTokIndex) + "on line " + std::to_string(program_tokens[curTokIndex].lineNo);
      throw std::runtime_error(err_msg);
    }
  }
}

// ASTnode root;
static void parser() {
  parse_general("start", 0);
}

/*
get the table parser_table[nonterm, term]
stack.push("EOF")
stack.push("start")
curTok is prev declared
while True:
  t = stack.pop()
  if t is a terminal:
    if t == curTok:
      curTok++
    elif t != curTok:
      raise Error
  elif t is a nonterminal:
    declare selected_production
    if parser_table[t, curTok] has zero productions then raise Error
    elif parser_table[t, curTok] has one production then that is selected_production
    elif parser_table[t, curTok] has multiple productions then
      based on t, curTok, curTok + 1, curtok + 2 choose selected_production appropriately
    let P1 ... Pk be selected_production
    push Pk, ..., P1 to the stack so P1 is on top
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