#include "AST/program.hpp"

// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                const char *const p_name, vector<DeclNode*> decls, vector<FunctionNode*> funcs,
                CompoundStatementNode *comp)
    : AstNode{line, col}, name(p_name), decls(decls), funcs(funcs), comp(comp) {}

void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // declaraions
    for(auto &decl : decls){
        decl->accept(p_visitor);
    }

    // functions
    for(auto &func : funcs){
        func->accept(p_visitor);
    }
    
    // componud statement
    comp->accept(p_visitor);
}
