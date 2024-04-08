#ifndef AST_VARIABLE_REFERENCE_NODE_H
#define AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>
#include <string>
#include <vector>

class VariableReferenceNode final : public ExpressionNode {
  public:
    using ExprNodes = std::vector<std::unique_ptr<ExpressionNode>>;
    bool has_valid_index = true;
    uint32_t invalid_index_line;
    uint32_t invalid_index_col;
    std::string kind;
    int table_index;

  private:
    std::string m_name;
    ExprNodes m_indices;

  public:
    ~VariableReferenceNode() = default;

    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          const char *const p_name)
        : ExpressionNode{line, col}, m_name(p_name){}

    // array reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          const char *const p_name, ExprNodes &p_indices)
        : ExpressionNode{line, col}, m_name(p_name),
          m_indices(std::move(p_indices)){}

    const char *getNameCString() const { return m_name.c_str(); }
    ExprNodes &getIndices() { return m_indices; }

    void accept(AstNodeVisitor &p_visitor) override {p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor);
};

#endif
