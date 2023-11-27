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
#include "lexer.h"
#include "astnodes.h"
#include "loaddata.h"
#include "parser.h"
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

FILE *pFile;

std::string IdentifierStr; // Filled in if IDENT
int IntVal;                // Filled in if INT_LIT
bool BoolVal;              // Filled in if BOOL_LIT
float FloatVal;            // Filled in if FLOAT_LIT
std::string StringVal;     // Filled in if String Literal
int lineNo, columnNo; // not static because used in mccomp.cpp
TOKEN CurTok;
std::deque<TOKEN> tok_buffer;

std::string grammarversion = "8";
std::string csvfilename = "./firstfollow/firstfollowg"+grammarversion+"sep.csv";
std::string grammarfilename = "./grammars/transformedgrammar"+grammarversion+".txt";
std::string terminalfilename = "./grammars/terminals2.txt";
std::vector<std::string> nonterminals; // this is the lhs of the grammar, does not include epsilon
std::vector<production_options> rhslist;
std::vector<std::string> terminals;
std::map<std::string, bool> nullable; //dictionaries for ifnullable, first and follow sets
std::map<std::string, std::vector<std::string>> first;
std::map<std::string, std::vector<std::string>> follow;

// AST Printer
string br = "|-";
string sp = " ";
string nl = "\n";
inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const ASTnode &ast) {
  os << ast.to_string("");
  return os;
}

// Code Generation
llvm::LLVMContext TheContext;
std::unique_ptr<llvm::Module> TheModule;
llvm::IRBuilder<> Builder(TheContext);

// Main driver code.
int main(int argc, char **argv) {
  if (argc == 2) {
    pFile = fopen(argv[1], "r");
    if (pFile == NULL)
      perror("Error opening file");
  } else {
    std::cout << "Usage: ./code InputFile\n";
    return 1;
  }

  lineNo = 1; // initialize line number and column numbers to zero
  columnNo = 1;
  load_data(); // load first and follow set from file, load list of terminals, load list of production lhs and rhs respectively
  getNextToken(); // get the first token
  std::unique_ptr<ProgramASTnode> programrootnode = parser(); // Run the parser now.
  fprintf(stderr, "Lexing and Parsing Finished\n");
  llvm::outs() << *programrootnode << "\n";
  fprintf(stderr, "Printing AST Finished\n");

  // Start printing final IR
  // Print out all of the generated code into a file called output.ll or to the terminal
  
  auto Filename = "output.ll";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);
  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }
  // Make the module, which holds all the code.
  TheModule = std::make_unique<llvm::Module>("mini-c", TheContext);
  // TheModule->llvm::Module::print(dest, nullptr); // print IR to file output.ll
  TheModule->print(llvm::errs(), nullptr); // print IR to terminal
  // End printing final IR
  fclose(pFile); // close the file that contains the code that was parsed
  return 0;
}

 // program_tokens.push_back(CurTok);
  // while (CurTok.type != EOF_TOK) {
  //   fprintf(stderr, "Token: %s with type %d\n", CurTok.lexeme.c_str(), CurTok.type);
  //   getNextToken();
  //   program_tokens.push_back(CurTok);
  // }
  // // remove the last EOF_TOK
  // program_tokens.pop_back();
  // fprintf(stderr, "Lexer Finished\n");
  // fprintf(stderr, "This is our token: %d\n", CurTok.type);

  // // print our deque of tokens
  // for (int i = 0; i<program_tokens.size(); i++){
  //   fprintf(stderr, "Token: %s with type %d\n", program_tokens[i].lexeme.c_str(), program_tokens[i].type);
  // }