#include "AST/return.hpp"

// TODO
ReturnNode::ReturnNode(const uint32_t line, const uint32_t col, ExpressionNode* expression)
    : AstNode{line, col}, expression(expression) {}

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expression->accept(p_visitor);
}
