#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include<string>
using namespace std;

class UnaryOperatorNode : public ExpressionNode {
  private:
    string op;
    ExpressionNode *expression;
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col,
                      string op, ExpressionNode *p_expression);
    ~UnaryOperatorNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    string getOperator() const { return op; }
};

#endif
