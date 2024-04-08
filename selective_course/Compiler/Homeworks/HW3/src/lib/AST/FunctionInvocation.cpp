#include "AST/FunctionInvocation.hpp"

// TODO
FunctionInvocationNode::FunctionInvocationNode(const uint32_t line, const uint32_t col,
                                               char* name,
                                               vector<ExpressionNode*> expressions)
    : ExpressionNode{line, col}, name(string(name)), expressions(expressions) {}

void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for(auto &expression : expressions){
        expression->accept(p_visitor);
    }
}
