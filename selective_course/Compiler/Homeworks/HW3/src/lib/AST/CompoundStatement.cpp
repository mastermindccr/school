#include "AST/CompoundStatement.hpp"
#include "AST/assignment.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/for.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/if.hpp"
#include "AST/return.hpp"
#include "AST/while.hpp"

// TODO
CompoundStatementNode::CompoundStatementNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col}{}

CompoundStatementNode::CompoundStatementNode(const uint32_t line, const uint32_t col,
                                             vector<DeclNode*> declarations,
                                             vector<AstNode*> statements)
    : AstNode{line, col}, declarations(declarations), statements(statements) {}

void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for(auto &decl : declarations) {
        decl->accept(p_visitor);
    }
    for(auto &decl : statements) {
        decl->accept(p_visitor);
    }
}
