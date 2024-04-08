#include "AST/while.hpp"

// TODO
WhileNode::WhileNode(const uint32_t line, const uint32_t col,
                     ExpressionNode *expression, CompoundStatementNode *body)
    : AstNode{line, col}, expression(expression), body(body) {}

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expression->accept(p_visitor);
    body->accept(p_visitor);
}
