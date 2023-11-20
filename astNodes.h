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
  IntASTnode(TOKEN tok, int val) : Tok(tok), Val(val){}
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


// class ProgramASTnode;
// class ExternASTnode;

// class VariableDeclASTnode;
// class FunctionDeclASTnode;

// class ArgASTnode;

// class BlockASTnode;

// class VariableAssignASTnode;
// class ExprASTnode;
// class BinOpASTnode;
// class UnaryOpASTnode;
// Class LiteralASTnode;
// class IfThenElseASTnode;
// class WhileASTnode;


// class IntASTnode;
// class BoolASTnode;
// class FloatASTnode;
// class IdentASTnode;
// class FunctionCallASTnode;


/* add other AST nodes as nessasary */

