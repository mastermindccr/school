#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>
#include <string>

using namespace std;

class BinaryOperatorNode : public ExpressionNode {
  private:
    string op;
    ExpressionNode *left;
    ExpressionNode *right;
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col,
                       string op, ExpressionNode *p_left, ExpressionNode *p_right);
    ~BinaryOperatorNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    string getOperator() { return op; }
};

#endif
