#include "AST/if.hpp"

// TODO
IfNode::IfNode(const uint32_t line, const uint32_t col,
               ExpressionNode *expression, CompoundStatementNode *body,
               CompoundStatementNode *else_body)
    : AstNode{line, col}, expression(expression), body(body), else_body(else_body) {}

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expression->accept(p_visitor);
    body->accept(p_visitor);
    if (else_body != NULL) {
        else_body->accept(p_visitor);
    }
}
