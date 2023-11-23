#pragma once
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "lexer.h"
#include "astnodes.h"
#include "loaddata.h"
#include <memory>
#include <exception>

using std::unique_ptr;

// extern std::deque<TOKEN> program_tokens;
// extern int curTokIndex;

extern TOKEN CurTok;
extern std::deque<TOKEN> tok_buffer;

// defined in loaddata.h
// typedef std::vector<std::string> sentence;
// typedef std::vector<sentence> production_options;

extern std::vector<std::string> nonterminals; // this is the lhs of the grammar, does not include epsilon
extern std::vector<production_options> rhslist;
extern std::vector<std::string> terminals;

//below are our dictionaries for first and follow sets
extern std::map<std::string, bool> nullable;
extern std::map<std::string, std::vector<std::string>> first;
extern std::map<std::string, std::vector<std::string>> follow;

extern unique_ptr<ProgramASTnode> programrootnode;

TOKEN getNextToken();
void putBackToken(TOKEN tok);

// int word_to_type(std::string word);
enum TOKEN_TYPE word_to_type(std::string word);
std::vector<int> terminals_to_int(std::vector<std::string> terminals);

unique_ptr<ProgramASTnode> parser();
unique_ptr<ProgramASTnode> parse_program();
unique_ptr<ExternListASTnode> parse_extern_list();
unique_ptr<DeclListASTnode> parse_decl_list();
unique_ptr<ExternASTnode> parse_extern();
unique_ptr<ExternListASTnode> parse_extern_list1();
unique_ptr<DeclASTnode> parse_decl();
unique_ptr<DeclListASTnode> parse_decl_list1();
unique_ptr<TypeSpecASTnode>parse_type_spec();
unique_ptr<ParamListASTnode> parse_params();
unique_ptr<VarDeclASTnode> parse_var_decl();
unique_ptr<FunDeclASTnode> parse_fun_decl();
unique_ptr<VarTypeASTnode> parse_var_type();
unique_ptr<BlockASTnode> parse_block();
unique_ptr<ParamListASTnode> parse_param_list();
unique_ptr<ParamASTnode> parse_param();
unique_ptr<ParamListASTnode> parse_param_list1();
unique_ptr<DeclListASTnode> parse_local_decls();
unique_ptr<DeclASTnode> parse_local_decl();
unique_ptr<StmtListASTnode> parse_stmt_list();
unique_ptr<StmtASTnode> parse_stmt();
unique_ptr<ExprASTnode> parse_expr_stmt();
unique_ptr<WhileASTnode> parse_while_stmt();
unique_ptr<IfASTnode> parse_if_stmt();
unique_ptr<ElseASTnode> parse_else_stmt();
unique_ptr<ReturnASTnode> parse_return_stmt();
unique_ptr<ExprASTnode> parse_expr();



class ParseError : public std::exception {
  TOKEN tok;
  std::string message = "";
public:
  ParseError(TOKEN t, std::string comment) : tok(t) {
    this->message += ("parsing error with token " + tok.lexeme + " of type " + std::to_string(tok.type) + " on line " + std::to_string(tok.lineNo) + ", column " + std::to_string(tok.columnNo) + "\n" + comment);
  }
  const char *what() const noexcept override {
    return message.c_str();
    // return "our error class caught the parsing error";
  }
};

// std::vector<std::string> find_sentence_first(sentence &sentence);
// bool sentencenullable(sentence &sentence);
// sentence look_ahead_special_case(std::string nonterminal, production_options productions);
// sentence choose_production(std::string nonterminal, production_options productions);
// void parse_general(std::string nonterminal, int depth);
// void parser();