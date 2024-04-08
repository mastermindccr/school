#include "AST/BinaryOperator.hpp"

// TODO
BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col,
                                       string op, ExpressionNode *p_left,
                                       ExpressionNode *p_right)
    : ExpressionNode{line, col}, op(op), left(p_left), right(p_right) {}


void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    left->accept(p_visitor);
    right->accept(p_visitor);
}
