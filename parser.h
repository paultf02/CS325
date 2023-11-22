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

extern std::deque<TOKEN> program_tokens;
extern int curTokIndex;

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

extern std::unique_ptr<ProgramASTnode> programrootnode;

TOKEN getNextToken();
void putBackToken(TOKEN tok);

// int word_to_type(std::string word);
enum TOKEN_TYPE word_to_type(std::string word);
std::vector<int> terminals_to_int(std::vector<std::string> terminals);

std::unique_ptr<ProgramASTnode> parser();
std::unique_ptr<ProgramASTnode> parse_program();

class ParseError : public std::exception {
  TOKEN tok;
  std::string message = "";
public:
  ParseError(TOKEN t) : tok(t) {
    this->message += ("parsing error with token " + tok.lexeme + " of type " + std::to_string(tok.type) + " on line " + std::to_string(tok.lineNo) + ", column " + std::to_string(tok.columnNo));
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