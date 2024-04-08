#ifndef AST_EXPRESSION_NODE_H
#define AST_EXPRESSION_NODE_H

#include "AST/ast.hpp"
#include <string>

using namespace std;

class ExpressionNode : public AstNode {
  public:
    ~ExpressionNode() = default;
    ExpressionNode(const uint32_t line, const uint32_t col)
        : AstNode{line, col} {}

    void accept(AstNodeVisitor &p_visitor) override {};

    // getter
    bool getIsValid() const { return is_valid; }
    string getBaseType() const { return base_type; }
    int getDimension() const { return dimension; }
    string getReturnType() const { return return_type; }

    // setter
    void setIsValid(bool valid) { is_valid = valid; }
    void setBaseType(string type) { base_type = type; }
    void setDimension(int dim) { dimension = dim; }
    void setReturnType(string type) { return_type = type; }

  protected:
    bool is_valid = true;
    string base_type = "";
    int dimension = 0;
    string return_type = "";
};

#endif
