#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/ast.hpp"
#include "AST/VariableReference.hpp"
#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class AssignmentNode : public AstNode {
  private:
    VariableReferenceNode *variable;
    ExpressionNode *expression;
  public:
    AssignmentNode(const uint32_t line, const uint32_t col,
                   VariableReferenceNode *variable, ExpressionNode *expression);
    ~AssignmentNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

};

#endif
