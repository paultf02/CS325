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


//===----------------------------------------------------------------------===//
// Recursive Descent Parser - Function call for each production
//===----------------------------------------------------------------------===//


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

//===----------------------------------------------------------------------===//
// Parser Buffer
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
/// we also need to build the parse tree





int word_to_type(std::string word){
  int type = -100;
  if (word=="IDENT"){
    type = -1;
  } else if (word=="'='"){
    type = int('=');
  } else if (word=="'{'"){
    type = int('{');
  } else if (word=="'}'"){
    type = int('}');
  } else if (word=="'('"){
    type = int('(');
  } else if (word=="')'"){
    type = int(')');
  } else if (word=="';'"){
    type = int(';');
  } else if (word=="','"){
    type = int(',');
  } else if (word=="'int'"){
    type = -2;
  } else if (word=="'void'"){
    type = -3;
  } else if (word=="'float'"){
    type = -4;
  } else if (word=="'bool'"){
    type = -5;
  } else if (word=="'extern'"){
    type = -6;
  } else if (word=="'if'"){
    type = -7;
  } else if (word=="'else'"){
    type = -8;
  } else if (word=="'while'"){
    type = -9;
  } else if (word=="'return'"){
    type = -10;
  } else if (word=="INT_LIT"){
    type = -14;
  } else if (word=="FLOAT_LIT"){
    type = -15;
  } else if (word=="BOOL_LIT"){
    type = -16;
  } else if (word=="'&&'"){
    type = -17;
  } else if (word=="'||'"){
    type = -18;
  } else if (word=="'+'"){
    type = int('+');
  } else if (word=="'-'"){
    type = int('-');
  } else if (word=="'*'"){
    type = int('*');
  } else if (word=="'/'"){
    type = int('/');
  } else if (word=="'%'"){
    type = int('%');
  } else if (word=="'!'"){
    type = int('!');
  } else if (word=="'=='"){
    type = -19;
  } else if (word=="'!='"){
    type = -20;
  } else if (word=="'<='"){
    type = -21;
  } else if (word=="'<'"){
    type = int('<');
  } else if (word=="'>='"){
    type = -23;
  } else if (word=="'>'"){
    type = int('>');
  } else if (word=="EOF"){
    type = 0;
  } else {
    type = -100;
  }
  return type;
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

std::vector<int> terminals_to_int(std::vector<std::string> list_of_terminals){
  std::vector<int> intvector(list_of_terminals.size());
  for (int i=0; i<list_of_terminals.size();i++){
    intvector[i] = word_to_type(list_of_terminals[i]);
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

void parser() {
  parse_general("start", 0);
}