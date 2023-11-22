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
 // 
// using namespace llvm;
// using namespace llvm::sys;

FILE *pFile;

std::string IdentifierStr; // Filled in if IDENT
int IntVal;                // Filled in if INT_LIT
bool BoolVal;              // Filled in if BOOL_LIT
float FloatVal;            // Filled in if FLOAT_LIT
std::string StringVal;     // Filled in if String Literal
int lineNo, columnNo; // not static because used in mccomp.cpp

std::deque<TOKEN> program_tokens;
int curTokIndex = 0;

TOKEN CurTok;
std::deque<TOKEN> tok_buffer;

std::vector<std::string> nonterminals; // this is the lhs of the grammar, does not include epsilon
std::vector<production_options> rhslist;
std::vector<std::string> terminals;
//below are our dictionaries for first and follow sets
std::map<std::string, bool> nullable;
std::map<std::string, std::vector<std::string>> first;
std::map<std::string, std::vector<std::string>> follow;

std::unique_ptr<ProgramASTnode> programrootnode;


//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;

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
      
  // Make the module, which holds all the code.
  TheModule = std::make_unique<llvm::Module>("mini-c", TheContext);

  // load first and follow set from file, load list of terminals, load list of production lhs and rhs respectively
  load_data();

  // Run the parser now.
  programrootnode = parser();
  fprintf(stderr, "Parsing Finished\n");
  // std::cout << std::to_string(nonterminal_index("block"))<<'\n';

  //********************* Start printing final IR **************************
  // Print out all of the generated code into a file called output.ll
  auto Filename = "output.ll";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }
  // TheModule->print(errs(), nullptr); // print IR to terminal
  TheModule->llvm::Module::print(dest, nullptr);
  //********************* End printing final IR ****************************

  fclose(pFile); // close the file that contains the code that was parsed
  return 0;
}
