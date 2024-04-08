#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <string>
using namespace std;

class ConstantValueNode : public ExpressionNode {
  private:
    string type;
    int int_val;
    float real_val;
    string str_and_bool_val;
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col, int int_val);
    ConstantValueNode(const uint32_t line, const uint32_t col, float real_val);
    ConstantValueNode(const uint32_t line, const uint32_t col, char* str_val);
    ConstantValueNode(const uint32_t line, const uint32_t col, bool bool_val);
    ~ConstantValueNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }

    string getType(){ return this->type; };
    int getIntVal(){ return this->int_val; };
    float getRealVal(){ return this->real_val; };
    string getStrAndBoolVal(){ return this->str_and_bool_val; };
};

#endif
