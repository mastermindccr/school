#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include "AST/decl.hpp"
#include "AST/CompoundStatement.hpp"
#include <vector>
#include <string>

using namespace std;

class FunctionNode : public AstNode {
  private:
    string name;
    vector<DeclNode*> declarations;
    string return_type;
    CompoundStatementNode* compound_statement;
    vector<string> var_types;

  public:
    FunctionNode(const uint32_t line, const uint32_t col, char* name, vector<DeclNode*> declarations,
                 char* return_type, CompoundStatementNode* compound_statement);
    ~FunctionNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    
    string getName(){ return name; };
    string getReturnType(){ return return_type; };
    vector<string> getVarTypes(){ return var_types; };
};

#endif
