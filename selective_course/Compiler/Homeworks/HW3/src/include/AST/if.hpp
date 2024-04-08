#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "AST/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"

class IfNode : public AstNode {
  private:
    ExpressionNode *expression;
    CompoundStatementNode *body;
    CompoundStatementNode *else_body;
  public:
    IfNode(const uint32_t line, const uint32_t col,
           ExpressionNode *p_expression, CompoundStatementNode *body,
           CompoundStatementNode *else_body);
    ~IfNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

};

#endif
