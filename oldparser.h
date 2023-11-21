#pragma once
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "lexer.h"
#include "astnodes.h"
#include "loaddata.h"

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

int word_to_type(std::string word);
TOKEN getNextToken();
void putBackToken(TOKEN tok);
std::vector<std::string> find_sentence_first(sentence &sentence);
std::vector<int> terminals_to_int(std::vector<std::string> terminals);
bool sentencenullable(sentence &sentence);
sentence look_ahead_special_case(std::string nonterminal, production_options productions);
sentence choose_production(std::string nonterminal, production_options productions);
void parse_general(std::string nonterminal, int depth);
void parser();
