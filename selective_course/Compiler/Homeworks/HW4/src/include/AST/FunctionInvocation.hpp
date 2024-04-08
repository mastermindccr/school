#ifndef AST_FUNCTION_INVOCATION_NODE_H
#define AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>
#include <string>
#include <vector>

class FunctionInvocationNode final : public ExpressionNode {
  public:
    using ExprNodes = std::vector<std::unique_ptr<ExpressionNode>>;

  private:
    std::string m_name;
    ExprNodes m_args;

  public:
    ~FunctionInvocationNode() = default;
    FunctionInvocationNode(const uint32_t line, const uint32_t col,
                           const char *const p_name, ExprNodes &p_args)
        : ExpressionNode{line, col}, m_name(p_name), m_args(std::move(p_args)){}

    const char *getNameCString() const { return m_name.c_str(); }
    vector<string> getArgsType() const {
        vector<string> args;
        for(int i = 0; i < (int)m_args.size(); i++){
            args.push_back(m_args[i]->getReturnType());
        }
        return args;
    }

    pair<int, int> getInvalidArgLocation(int index){
        return make_pair(m_args[index]->getLocation().line, m_args[index]->getLocation().col);
    }

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif
