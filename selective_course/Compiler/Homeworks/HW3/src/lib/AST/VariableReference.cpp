#include "AST/VariableReference.hpp"

// TODO
VariableReferenceNode::VariableReferenceNode(const uint32_t line, const uint32_t col,
                                             char* p_name,
                                             vector<ExpressionNode*> expressions)
    : ExpressionNode{line, col}, name(string(p_name)), expressions(expressions) {}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &expression : expressions) {
        expression->accept(p_visitor);
    }
}
