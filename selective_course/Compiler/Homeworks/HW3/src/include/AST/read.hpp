#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/ast.hpp"
#include "AST/VariableReference.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ReadNode : public AstNode {
  private:
    VariableReferenceNode* var_ref;
  public:
    ReadNode(const uint32_t line, const uint32_t col, VariableReferenceNode* var_ref);
    ~ReadNode() = default;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

};

#endif
