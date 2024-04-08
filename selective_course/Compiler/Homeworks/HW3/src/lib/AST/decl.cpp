#include "AST/decl.hpp"
#include <cstdio>

// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col, vector<VariableNode*> variables, char* type)
    : AstNode{line, col}, variables(variables) {
        for(auto &var : variables){
            var->setType(type);
            var->set_const_value_node(nullptr);
        }
    }

DeclNode::DeclNode(const uint32_t line, const uint32_t col, vector<VariableNode*> variables, ConstantValueNode* const_value)
    : AstNode{line, col}, variables(variables) {
        for(auto &var : variables){
            var->setType((char*)const_value->getType().c_str());
            var->set_const_value_node(const_value);
        }
    }

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for(auto &var : variables){
        var->accept(p_visitor);
    }
}
