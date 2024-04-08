#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/assignment.hpp"
#include "AST/expression.hpp"
#include "AST/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ForNode : public AstNode {
  private:
    DeclNode *declaration;
    AssignmentNode *assignment;
    ExpressionNode *expression;
    CompoundStatementNode *body;
  public:
    ForNode(const uint32_t line, const uint32_t col,
            DeclNode *declaration, AssignmentNode *assignment,
            ExpressionNode *expression, CompoundStatementNode *body);
    ~ForNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

};

#endif
