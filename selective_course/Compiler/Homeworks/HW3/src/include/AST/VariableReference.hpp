#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <string>
#include <vector>
using namespace std;

class VariableReferenceNode : public ExpressionNode {
  private:
    string name;
    vector<ExpressionNode*> expressions;

  public:
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          char* name, vector<ExpressionNode*> expressions);

    ~VariableReferenceNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    string getName() { return name; }
};

#endif
