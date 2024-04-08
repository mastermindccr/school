#include "AST/UnaryOperator.hpp"

// TODO
UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col,
                                     string op, ExpressionNode *expression)
    : ExpressionNode{line, col}, op(op), expression(expression) {}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expression->accept(p_visitor);
}
