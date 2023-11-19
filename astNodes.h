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
