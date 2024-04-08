#ifndef AST_PROGRAM_NODE_H
#define AST_PROGRAM_NODE_H

#include "AST/ast.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/decl.hpp"
#include "AST/function.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include <memory>

#include <string>
#include <vector>

using namespace std;

class ProgramNode final : public AstNode {
  private:
    string name;
    vector<DeclNode*> decls;
    vector<FunctionNode*> funcs;
    CompoundStatementNode *comp;

  public:
    ~ProgramNode() = default;
    ProgramNode(const uint32_t line, const uint32_t col,
                const char *const p_name, vector<DeclNode*> decls, vector<FunctionNode*> funcs,
                CompoundStatementNode *comp);

    const char *getNameCString() const { return name.c_str(); }
    
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif
