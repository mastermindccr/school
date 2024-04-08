#include "AST/expression.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/VariableReference.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/ConstantValue.hpp"

ExpressionNode::ExpressionNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}

void ExpressionNode::accept(AstNodeVisitor &p_visitor){
    if(BinaryOperatorNode* p = dynamic_cast<BinaryOperatorNode*>(this)){
        p_visitor.visit(*p);
    }
    else if(UnaryOperatorNode* p = dynamic_cast<UnaryOperatorNode*>(this)){
        p_visitor.visit(*p);
    }
    else if(VariableReferenceNode* p = dynamic_cast<VariableReferenceNode*>(this)){
        p_visitor.visit(*p);
    }
    else if(FunctionInvocationNode* p = dynamic_cast<FunctionInvocationNode*>(this)){
        p_visitor.visit(*p);
    }
    else if(ConstantValueNode* p = dynamic_cast<ConstantValueNode*>(this)){
        p_visitor.visit(*p);
    }
    else if(UnaryOperatorNode* p = dynamic_cast<UnaryOperatorNode*>(this)){
        p_visitor.visit(*p);
    }
    else if(FunctionInvocationNode *p = dynamic_cast<FunctionInvocationNode*>(this)){
        p_visitor.visit(*p);
    }
}
