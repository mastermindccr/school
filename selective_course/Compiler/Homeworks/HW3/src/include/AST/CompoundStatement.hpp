#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "visitor/AstNodeVisitor.hpp"

class CompoundStatementNode : public AstNode {
  private:
    vector<DeclNode*> declarations;
    vector<AstNode*> statements;

  public:
  CompoundStatementNode(const uint32_t line, const uint32_t col);
    CompoundStatementNode(const uint32_t line, const uint32_t col, vector<DeclNode*> declarations, vector<AstNode*> statements);
    ~CompoundStatementNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    // TODO: declarations, statements
};

#endif
