#include "AST/variable.hpp"
#include <cstdio>

// TODO
VariableNode::VariableNode(const uint32_t line, const uint32_t col, char* var_name)
    : AstNode{line, col} {
        this->var_name = string(var_name);
    }

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(const_value!=nullptr){
        const_value->accept(p_visitor);
    }
}
