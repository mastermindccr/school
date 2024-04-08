#include "AST/ConstantValue.hpp"
#include "stdio.h"

// TODO
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, int int_val)
    : ExpressionNode{line, col}, type("integer") {
        this->int_val = int_val;
    }

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, float real_val)
    : ExpressionNode{line, col}, type("real") {
        this->real_val = real_val; 
    }

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, char* str_val)
    : ExpressionNode{line, col}, type("string") {
        this->str_and_bool_val = string(str_val);
    }

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, bool bool_val)
    : ExpressionNode{line, col}, type("boolean") {
        if(bool_val){
            this->str_and_bool_val = "true";
        }
        else{
            this->str_and_bool_val = "false";
        }
    }
