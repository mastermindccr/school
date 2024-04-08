#include "AST/for.hpp"

// TODO
ForNode::ForNode(const uint32_t line, const uint32_t col,
                 DeclNode *declaration, AssignmentNode *assignment,
                 ExpressionNode *expression, CompoundStatementNode *body)
    : AstNode{line, col}, declaration(declaration), assignment(assignment),
        expression(expression), body(body) {}

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    declaration->accept(p_visitor);
    assignment->accept(p_visitor);
    expression->accept(p_visitor);
    body->accept(p_visitor);
}
