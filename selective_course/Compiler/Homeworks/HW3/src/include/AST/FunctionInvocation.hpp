#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <string>
#include <vector>

using namespace std;

class FunctionInvocationNode : public ExpressionNode {
  private:
    string name;
    std::vector<ExpressionNode*> expressions;
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col,
                           char* name, std::vector<ExpressionNode*> expressions);
    ~FunctionInvocationNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    string getName() { return name; }
};

#endif
