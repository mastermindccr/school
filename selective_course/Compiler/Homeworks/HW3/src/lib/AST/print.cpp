#include "AST/print.hpp"

// TODO
PrintNode::PrintNode(const uint32_t line, const uint32_t col, ExpressionNode *p_expression)
    : AstNode{line, col} {
        expression = p_expression;
    }

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expression->accept(p_visitor);
}
