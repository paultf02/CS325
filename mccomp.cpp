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

bool str_to_bool(std::string word){
  if (word == "True"){
    return true;
  } else if (word == "False"){
    return false;
  } else {
    throw std::runtime_error("cannot convert string to bool");
  }
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
  std::string csvfilename, terminalfilename, grammarfilename;
  csvfilename = "firstfollowg8sep.csv";
  terminalfilename = "terminals2.txt";
  grammarfilename = "transformedgrammar8.txt";
  std::fstream csvfile, terminalfile, grammarfile;
  std::string line, part;
  std::string name, isnullable, firstset, followset, lhs, rhs, or_sequence;
  char sep = '?';

  csvfile.open(csvfilename, std::ios::in);
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

  terminalfile.open(terminalfilename, std::ios::in);
  if (!terminalfile) {                        
    std::cout<<"File doesn’t exist.";
    throw std::runtime_error("could not open file");         
  }
  while (std::getline(terminalfile, line)){
    terminals.push_back(line);
  }
  terminalfile.close();

  grammarfile.open(grammarfilename, std::ios::in);
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




// program ::= extern_list decl_list







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
