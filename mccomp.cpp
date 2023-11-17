#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string.h>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

using namespace llvm;
using namespace llvm::sys;

FILE *pFile;

//edit

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns one of these for known things.
enum TOKEN_TYPE {

  IDENT = -1,        // [a-zA-Z_][a-zA-Z_0-9]*
  ASSIGN = int('='), // '='

  // delimiters
  LBRA = int('{'),  // left brace
  RBRA = int('}'),  // right brace
  LPAR = int('('),  // left parenthesis
  RPAR = int(')'),  // right parenthesis
  SC = int(';'),    // semicolon
  COMMA = int(','), // comma

  // types
  INT_TOK = -2,   // "int"
  VOID_TOK = -3,  // "void"
  FLOAT_TOK = -4, // "float"
  BOOL_TOK = -5,  // "bool"

  // keywords
  EXTERN = -6,  // "extern"
  IF = -7,      // "if"
  ELSE = -8,    // "else"
  WHILE = -9,   // "while"
  RETURN = -10, // "return"
  // TRUE   = -12,     // "true"
  // FALSE   = -13,     // "false"

  // literals
  INT_LIT = -14,   // [0-9]+
  FLOAT_LIT = -15, // [0-9]+.[0-9]+
  BOOL_LIT = -16,  // "true" or "false" key words

  // logical operators
  AND = -17, // "&&"
  OR = -18,  // "||"

  // operators
  PLUS = int('+'),    // addition or unary plus
  MINUS = int('-'),   // substraction or unary negative
  ASTERIX = int('*'), // multiplication
  DIV = int('/'),     // division
  MOD = int('%'),     // modular
  NOT = int('!'),     // unary negation

  // comparison operators
  EQ = -19,      // equal
  NE = -20,      // not equal
  LE = -21,      // less than or equal to
  LT = int('<'), // less than
  GE = -23,      // greater than or equal to
  GT = int('>'), // greater than

  // special tokens
  EOF_TOK = 0, // signal end of file

  // invalid
  INVALID = -100 // signal invalid token
};

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
    type = int('_');
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

bool str_to_bool(std::string word){
  if (word == "True"){
    return true;
  } else if (word == "False"){
    return false;
  } else {
    throw std::runtime_error("cannot convert string to bool");
  }
}
// TOKEN struct is used to keep track of information about a token
struct TOKEN {
  int type = -100;
  std::string lexeme;
  int lineNo;
  int columnNo;
};

static std::string IdentifierStr; // Filled in if IDENT
static int IntVal;                // Filled in if INT_LIT
static bool BoolVal;              // Filled in if BOOL_LIT
static float FloatVal;            // Filled in if FLOAT_LIT
static std::string StringVal;     // Filled in if String Literal
static int lineNo, columnNo;

static TOKEN returnTok(std::string lexVal, int tok_type) {
  TOKEN return_tok;
  return_tok.lexeme = lexVal;
  return_tok.type = tok_type;
  return_tok.lineNo = lineNo;
  return_tok.columnNo = columnNo - lexVal.length() - 1;
  return return_tok;
}

// Read file line by line -- or look for \n and if found add 1 to line number
// and reset column number to 0
/// gettok - Return the next token from standard input.
static TOKEN gettok() {

  static int LastChar = ' ';
  static int NextChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar)) {
    if (LastChar == '\n' || LastChar == '\r') {
      lineNo++;
      columnNo = 1;
    }
    LastChar = getc(pFile);
    columnNo++;
  }

  if (isalpha(LastChar) || (LastChar == '_')) { // identifier: [a-zA-Z_][a-zA-Z_0-9]*
    IdentifierStr = LastChar;
    columnNo++;

    while (isalnum((LastChar = getc(pFile))) || (LastChar == '_')) {
      IdentifierStr += LastChar;
      columnNo++;
    }

    if (IdentifierStr == "int")
      return returnTok("int", INT_TOK);
    if (IdentifierStr == "bool")
      return returnTok("bool", BOOL_TOK);
    if (IdentifierStr == "float")
      return returnTok("float", FLOAT_TOK);
    if (IdentifierStr == "void")
      return returnTok("void", VOID_TOK);
    if (IdentifierStr == "bool")
      return returnTok("bool", BOOL_TOK);
    if (IdentifierStr == "extern")
      return returnTok("extern", EXTERN);
    if (IdentifierStr == "if")
      return returnTok("if", IF);
    if (IdentifierStr == "else")
      return returnTok("else", ELSE);
    if (IdentifierStr == "while")
      return returnTok("while", WHILE);
    if (IdentifierStr == "return")
      return returnTok("return", RETURN);
    if (IdentifierStr == "true") {
      BoolVal = true;
      return returnTok("true", BOOL_LIT);
    }
    if (IdentifierStr == "false") {
      BoolVal = false;
      return returnTok("false", BOOL_LIT);
    }

    return returnTok(IdentifierStr.c_str(), IDENT);
  }

  if (LastChar == '=') {
    NextChar = getc(pFile);
    if (NextChar == '=') { // EQ: ==
      LastChar = getc(pFile);
      columnNo += 2;
      return returnTok("==", EQ);
    } else {
      LastChar = NextChar;
      columnNo++;
      return returnTok("=", ASSIGN);
    }
  }

  if (LastChar == '{') {
    LastChar = getc(pFile);
    columnNo++;
    return returnTok("{", LBRA);
  }
  if (LastChar == '}') {
    LastChar = getc(pFile);
    columnNo++;
    return returnTok("}", RBRA);
  }
  if (LastChar == '(') {
    LastChar = getc(pFile);
    columnNo++;
    return returnTok("(", LPAR);
  }
  if (LastChar == ')') {
    LastChar = getc(pFile);
    columnNo++;
    return returnTok(")", RPAR);
  }
  if (LastChar == ';') {
    LastChar = getc(pFile);
    columnNo++;
    return returnTok(";", SC);
  }
  if (LastChar == ',') {
    LastChar = getc(pFile);
    columnNo++;
    return returnTok(",", COMMA);
  }

  if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9]+.
    std::string NumStr;

    if (LastChar == '.') { // Floatingpoint Number: .[0-9]+
      do {
        NumStr += LastChar;
        LastChar = getc(pFile);
        columnNo++;
      } while (isdigit(LastChar));

      FloatVal = strtof(NumStr.c_str(), nullptr);
      return returnTok(NumStr, FLOAT_LIT);
    } else {
      do { // Start of Number: [0-9]+
        NumStr += LastChar;
        LastChar = getc(pFile);
        columnNo++;
      } while (isdigit(LastChar));

      if (LastChar == '.') { // Floatingpoint Number: [0-9]+.[0-9]+)
        do {
          NumStr += LastChar;
          LastChar = getc(pFile);
          columnNo++;
        } while (isdigit(LastChar));

        FloatVal = strtof(NumStr.c_str(), nullptr);
        return returnTok(NumStr, FLOAT_LIT);
      } else { // Integer : [0-9]+
        IntVal = strtod(NumStr.c_str(), nullptr);
        return returnTok(NumStr, INT_LIT);
      }
    }
  }

  if (LastChar == '&') {
    NextChar = getc(pFile);
    if (NextChar == '&') { // AND: &&
      LastChar = getc(pFile);
      columnNo += 2;
      return returnTok("&&", AND);
    } else {
      LastChar = NextChar;
      columnNo++;
      return returnTok("&", int('&'));
    }
  }

  if (LastChar == '|') {
    NextChar = getc(pFile);
    if (NextChar == '|') { // OR: ||
      LastChar = getc(pFile);
      columnNo += 2;
      return returnTok("||", OR);
    } else {
      LastChar = NextChar;
      columnNo++;
      return returnTok("|", int('|'));
    }
  }

  if (LastChar == '!') {
    NextChar = getc(pFile);
    if (NextChar == '=') { // NE: !=
      LastChar = getc(pFile);
      columnNo += 2;
      return returnTok("!=", NE);
    } else {
      LastChar = NextChar;
      columnNo++;
      return returnTok("!", NOT);
      ;
    }
  }

  if (LastChar == '<') {
    NextChar = getc(pFile);
    if (NextChar == '=') { // LE: <=
      LastChar = getc(pFile);
      columnNo += 2;
      return returnTok("<=", LE);
    } else {
      LastChar = NextChar;
      columnNo++;
      return returnTok("<", LT);
    }
  }

  if (LastChar == '>') {
    NextChar = getc(pFile);
    if (NextChar == '=') { // GE: >=
      LastChar = getc(pFile);
      columnNo += 2;
      return returnTok(">=", GE);
    } else {
      LastChar = NextChar;
      columnNo++;
      return returnTok(">", GT);
    }
  }

  if (LastChar == '/') { // could be division or could be the start of a comment
    LastChar = getc(pFile);
    columnNo++;
    if (LastChar == '/') { // definitely a comment
      do {
        LastChar = getc(pFile);
        columnNo++;
      } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

      if (LastChar != EOF)
        return gettok();
    } else
      return returnTok("/", DIV);
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF) {
    columnNo++;
    return returnTok("0", EOF_TOK);
  }

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  std::string s(1, ThisChar);
  LastChar = getc(pFile);
  columnNo++;
  return returnTok(s, int(ThisChar));
}

//===----------------------------------------------------------------------===//
// Parser Buffer
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
/// we also need to build the parse tree



static TOKEN CurTok;
static std::deque<TOKEN> tok_buffer;

static TOKEN getNextToken() {

  if (tok_buffer.size() == 0)
    tok_buffer.push_back(gettok());

  TOKEN temp = tok_buffer.front();
  tok_buffer.pop_front();

  return CurTok = temp;
}

static void putBackToken(TOKEN tok) {
  tok_buffer.push_front(tok);
}

//===----------------------------------------------------------------------===//
// AST nodes
//===----------------------------------------------------------------------===//

/// ASTnode - Base class for all AST nodes.
class ASTnode {
public:
  virtual ~ASTnode() {}
  virtual Value *codegen() = 0;
  virtual std::string to_string() const {
    return "";
  };
};

/// IntASTnode - Class for integer literals like 1, 2, 10,
class IntASTnode : public ASTnode {
  int Val;
  TOKEN Tok;
  std::string Name;

public:
  IntASTnode(TOKEN tok, int val) : Val(val), Tok(tok) {}
  virtual Value *codegen() override;
  // virtual std::string to_string() const override {
  // return a sting representation of this AST node
  //};
};

class GenericASTnode: public ASTnode {
  public:
    std::vector<ASTnode*> children;
};
// first let us do a parse where everything is an ASTnode. just get a basic tree sorted
// 

// class BoolASTnode;
// class FloatASTnode;
// class FunctionDeclASTnode;
// class FunctionCallASTnode;
// class VariableDeclASTnode;
// class BinOpASTnode;
// class IfThenElseASTnode;
// class WhileASTnode;
// class ExpressionASTnode;


/* add other AST nodes as nessasary */

//===----------------------------------------------------------------------===//
// Recursive Descent Parser - Function call for each production
//===----------------------------------------------------------------------===//

// The tokens are stored in program_tokens

// each element of rhslist is rhs which is a list of sentences (each sentence for each or)
// std::vector<string> rhs;
// rhs is the list of atoms in the sentence

typedef std::vector<std::string> sentence;
typedef std::vector<sentence> production_options;
std::vector<std::string> nonterminals; // this is the lhs of the grammar, does not include epsilon
std::vector<production_options> rhslist;
std::vector<std::string> terminals;

//below are our dictionaries for first and follow sets
std::map<std::string, bool> nullable;
std::map<std::string, std::vector<std::string>> first;
std::map<std::string, std::vector<std::string>> follow;

// make a queue of tokens
static std::deque<TOKEN> program_tokens;
static int curTokIndex = 0;

static int nonterminal_index(std::string term){
  auto it = std::find(nonterminals.begin(), nonterminals.end(), term);
  if (it == nonterminals.end()){
    // name not in vector
    throw std::runtime_error("this term is not in the terminal list");
  } else {
    auto index = std::distance(nonterminals.begin(), it);
    return index;
  }
}

/*
The below is ChatGPT3.5 code with the prompt
"write cpp code that takes a string and splits it delimited by commas. make sure to escape out commas in single quotes."
there were some minor errors with the generated code so i changed some parts
*/
std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> substrings;
    std::string currentSubstring;

    bool insideSingleQuotes = false;

    for (char c : input) {
        if (c == '\'') {
            insideSingleQuotes = !insideSingleQuotes;
            currentSubstring += c;
        } else if (c == ',' && !insideSingleQuotes) {
            // If it's a comma and not inside single quotes, start a new substring
            substrings.push_back(currentSubstring);
            currentSubstring.clear();
        } else {
            // Otherwise, append the character to the current substring
            currentSubstring += c;
        }
    }

    // Add the last substring after the loop
    substrings.push_back(currentSubstring);

    return substrings;
}

// chatgpt3.5 
// "write cpp code to split a string based on a string delimiter"
std::vector<std::string> splitString_strdelim(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while ((end = input.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
    }

    // Add the last token (or the only token if there is no delimiter)
    tokens.push_back(input.substr(start));

    return tokens;
}

void load_data(){
  std::fstream csvfile, terminalfile, grammarfile;
  std::string line, part;
  std::string name, isnullable, firstset, followset, lhs, rhs, or_sequence;
  char sep = '?';

  csvfile.open("firstfollowseparator.csv", std::ios::in);
  if (!csvfile) {                        
    std::cout<<"File doesn’t exist.";
    throw std::runtime_error("could not open file");         
  }
  // first line
  std::getline(csvfile, line); 
  // Read data, line by line
  while (std::getline(csvfile, line)){
    std::stringstream ss(line);
    std::getline(ss, name, sep);
    std::getline(ss, isnullable, sep);
    std::getline(ss, firstset, sep);
    std::getline(ss, followset, sep);

    firstset = firstset.substr(1, firstset.length()-2);
    followset = followset.substr(1, followset.length()-2);
    first.insert({name, splitString(firstset)});
    follow.insert({name, splitString(followset)});
    if (isnullable == "True"){
      nullable.insert({name, true});
    } else if (isnullable == "False"){
      nullable.insert({name, false});
    }
  }
  csvfile.close();

  terminalfile.open("terminals2.txt", std::ios::in);
  if (!terminalfile) {                        
    std::cout<<"File doesn’t exist.";
    throw std::runtime_error("could not open file");         
  }
  while (std::getline(terminalfile, line)){
    terminals.push_back(line);
  }
  terminalfile.close();

  grammarfile.open("transformedgrammar5.txt", std::ios::in);
  if (!grammarfile) {                        
    std::cout<<"File doesn’t exist.";
    throw std::runtime_error("could not open grammar file");         
  }
  while (std::getline(grammarfile, line)){
    // std::stringstream ss(line);
    // std::getline(line, lhs, " -> ");
    // std::cout << lhs << '\n';

    std::vector<std::string> splitline = splitString_strdelim(line, " -> ");
    std::string lhs = splitline[0];
    std::string rhs = splitline[1];

    nonterminals.push_back(lhs);
    std::vector<std::string> splitor = splitString_strdelim(rhs, " | ");


    // std::cout << "These are the elements of splitline:\n";
    // for (auto &elem : splitline){
    //   std::cout << elem << '\n';
    // }
    // std::cout << "These are the elements of splitor:\n";
    // for (auto &elem : splitor){
    //   std::cout << elem << '\n';
    // }

    production_options rhstyped;
    sentence thissentence;
    for (auto &elem: splitor){
      thissentence = splitString_strdelim(elem, " ");
      rhstyped.push_back(thissentence);
      // std::cout << "These are the elements of this sentence:\n";
      // for (auto &elem : thissentence){
      //   std::cout << elem << '\n';
      // }
    }
    rhslist.push_back(rhstyped);


  }
  grammarfile.close();

  // name = "expr";
  // std::cout << name <<" first:\n";
  // for (auto &elem : first[name]){
  //   std::cout << elem << '\n';
  // }
  // std::cout << name <<" follow:\n";
  // for (auto &elem : follow[name]){
  //   std::cout << elem << '\n';
  // }

  // std::cout << "iterating through terminals: " << '\n';
  // for (auto &elem : terminals){
  //   std::cout << elem << '\n';
  // }
}

/* Add function calls for each production */

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
  std::vector<std::string> sentence_first;
  for (auto &word : sentence){
    std::vector<std::string> thisfirst = first[word];
    sentence_first.insert(sentence_first.end(), thisfirst.begin(), thisfirst.end());
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
  if (nonterminal == "decl"){

  } else if (nonterminal == "return_stmt"){

  } else if (nonterminal == "expr"){
    
  }
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
    std::vector<int> sentencefirst = terminals_to_int(sentencefirstold);
    if (std::find(sentencefirst.begin(), sentencefirst.end(), program_tokens[curTokIndex].type) != sentencefirst.end()){
      numfirstsets += 1;
      lastsetindex = i;
    }
  }

  if (numfirstsets == 1){
      answer =  productions[lastsetindex];
  } else if (numfirstsets > 1){
      std::cout << "this is the uncompleted path\n";
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
        else if (numnullable ==0){
          throw std::runtime_error("there is a syntax error");
        }
      }
  } else {
    throw std::runtime_error("error");
  }
  return answer;
}



void parse_general(std::string nonterminal){
  std::cout << nonterminal << '\n';
  if (nonterminal=="epsilon"){
    return;
  }
  production_options productions = rhslist[nonterminal_index(nonterminal)];
  //note it will only have one
  // find some way of choosing this
  sentence prod = choose_production(nonterminal, productions);

  for (int i=0; i<prod.size(); i++){
    if (std::find(terminals.begin(), terminals.end(), prod[i]) == terminals.end()){
      // prod[i] is not a terminal 
      parse_general(prod[i]);
    } else if (word_to_type(prod[i]) == program_tokens[curTokIndex].type){
      // prod[i] is a terminal and matches the current token
      curTokIndex++;
      std::cout << "next token\n";
      // curNode.children = something;
    } else {
      // prod[i] is a terminal but doesn't match current token
      std::string err_msg = "compiling error with token number " + std::to_string(curTokIndex);
      throw std::runtime_error(err_msg);
    }
  }
}

// ASTnode root;
static void parser() {
  parse_general("start");
}



// program ::= extern_list decl_list



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




//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;

//===----------------------------------------------------------------------===//
// AST Printer
//===----------------------------------------------------------------------===//

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const ASTnode &ast) {
  os << ast.to_string();
  return os;
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//




int main(int argc, char **argv) {
  if (argc == 2) {
    pFile = fopen(argv[1], "r");
    if (pFile == NULL)
      perror("Error opening file");
  } else {
    std::cout << "Usage: ./code InputFile\n";
    return 1;
  }

  // initialize line number and column numbers to zero
  lineNo = 1;
  columnNo = 1;

  

  // if we are parsing and lexing at the same time then we do not do this part of the code.
  // get the first token
  getNextToken();
  program_tokens.push_back(CurTok);
  while (CurTok.type != EOF_TOK) {
    fprintf(stderr, "Token: %s with type %d\n", CurTok.lexeme.c_str(), CurTok.type);
    getNextToken();
    program_tokens.push_back(CurTok);
  }
  // remove the last EOF_TOK
  program_tokens.pop_back();
  fprintf(stderr, "Lexer Finished\n");
  // fprintf(stderr, "This is our token: %d\n", CurTok.type);

  // // print our deque of tokens
  // for (int i = 0; i<program_tokens.size(); i++){
  //   fprintf(stderr, "Token: %s with type %d\n", program_tokens[i].lexeme.c_str(), program_tokens[i].type);
  // }
      
  // Make the module, which holds all the code.
  TheModule = std::make_unique<Module>("mini-c", TheContext);

  // load first and follow set from file, load list of terminals, load list of production lhs and rhs respectively
  load_data();


  // Run the parser now.
  parser();
  fprintf(stderr, "Parsing Finished\n");
  // std::cout << std::to_string(nonterminal_index("block"))<<'\n';

  //********************* Start printing final IR **************************
  // Print out all of the generated code into a file called output.ll
  auto Filename = "output.ll";
  std::error_code EC;
  raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

  if (EC) {
    errs() << "Could not open file: " << EC.message();
    return 1;
  }
  // TheModule->print(errs(), nullptr); // print IR to terminal
  TheModule->print(dest, nullptr);
  //********************* End printing final IR ****************************

  fclose(pFile); // close the file that contains the code that was parsed
  return 0;
}
