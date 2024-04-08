#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <vector>

using namespace std;

class DeclNode : public AstNode {
  private:
    vector<VariableNode*> variables;

  public:
    // variable declaration
    DeclNode(const uint32_t line, const uint32_t col, vector<VariableNode*> variables, char* type);
    DeclNode(const uint32_t line, const uint32_t col, vector<VariableNode*> variables, ConstantValueNode* const_value);

    ~DeclNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    
    vector<string> getVariablesType(){
      vector<string> ret;
      for(auto &var : variables){
        ret.push_back(var->getType());
      }
      return ret;
    };

};

#endif
