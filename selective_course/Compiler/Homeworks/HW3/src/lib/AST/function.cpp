#include "AST/function.hpp"

// TODO
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col, char* name,
                           vector<DeclNode*> declarations, char* return_type,
                           CompoundStatementNode* compound_statement)
    : AstNode{line, col}{
        this->name = string(name);
        this->return_type = string(return_type);
        this->declarations = declarations;
        this->compound_statement = compound_statement;
        for(auto &decl: declarations){
            for(auto &var: decl->getVariablesType()){
                var_types.push_back(var);
            }
        }
    }

void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for(auto &decl : declarations){
        decl->accept(p_visitor);
    }
    if(compound_statement!=nullptr){
        compound_statement->accept(p_visitor);
    }
}
