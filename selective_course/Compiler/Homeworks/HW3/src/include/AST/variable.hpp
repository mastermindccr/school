#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ast.hpp"
#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <string>
using namespace std;

class VariableNode : public AstNode {
  private:
    string var_name;
    string type;
    ConstantValueNode* const_value;
  public:
    VariableNode(const uint32_t line, const uint32_t col, char* var_name);
    ~VariableNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    
    string get_var_name(){ return var_name; };
    string getType(){ return type; };
    void setType(char* type){ this->type = type; };
    void set_const_value_node(ConstantValueNode* const_value){ this->const_value = const_value; };
};

#endif
