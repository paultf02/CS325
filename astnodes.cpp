#include "astnodes.h"
#include <string>

using std::string;

string StmtASTnode::to_string(string pre) const {
  int i = 1;
  string npre = pre;
  // string ans = pre + "StmtASTnode: " + whichtype;
  string ans = "this should not be seen";
  if (whichtype=="expr_stmt"){
      ans = expr_stmt->to_string(npre);
  } else if (whichtype=="block"){
      // ans = block->to_string(npre);
      ans = npre + "block node" + nl;
  } else if (whichtype=="if_stmt"){
      ans = if_stmt->to_string(npre);
  } else if (whichtype=="while_stmt"){
      ans = while_stmt->to_string(npre);
  } else if (whichtype=="return_stmt"){
      ans = return_stmt->to_string(npre);
  };
  return ans;
};

string BlockASTnode::to_string(string pre) const {
  string npre = pre + sp;
  string ans = pre + "BlockASTnode:" + nl;
  // ans += npre + "local decls here" + nl;
  for (auto &ld : localdecls){
    ans += ld->to_string(npre);
  }
  for (auto &stmt : stmtlist){
    ans += stmt->to_string(npre);
  }    
  return ans;
};