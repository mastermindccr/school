#include "AST/read.hpp"

// TODO
ReadNode::ReadNode(const uint32_t line, const uint32_t col, VariableReferenceNode* var_ref)
    : AstNode{line, col}, var_ref(var_ref) {}

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    var_ref->accept(p_visitor);
}