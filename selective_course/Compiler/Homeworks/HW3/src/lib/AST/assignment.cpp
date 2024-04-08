#include "AST/assignment.hpp"

// TODO
AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,
                               VariableReferenceNode *variable,
                               ExpressionNode *expression)
    : AstNode{line, col}, variable(variable), expression(expression) {}


void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    variable->accept(p_visitor);
    expression->accept(p_visitor);
}
