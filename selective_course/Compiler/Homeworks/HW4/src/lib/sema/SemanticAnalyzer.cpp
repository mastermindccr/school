#include "sema/SemanticAnalyzer.hpp"
#include "visitor/AstNodeInclude.hpp"

extern char *lines[205];
extern bool sem_err;
extern uint32_t opt_sem;

static void dumpErrorMessage(uint32_t line_num, uint32_t col_num, string error_msg){
    fprintf(stderr, "<Error> Found in line %d, column %d: %s\n", line_num, col_num, error_msg.c_str());
                fprintf(stderr, "%*s%s\n", 4, "", lines[line_num]);
                fprintf(stderr, "%*s\n", col_num+4, "^");
                sem_err = true;
}

void SemanticAnalyzer::dumpDemarcation(const char chr) {
  for (size_t i = 0; i < 110; ++i) {
    printf("%c", chr);
  }
  puts("");
}

void SemanticAnalyzer::dumpSymbol(){
    dumpDemarcation('=');
    printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type",
                                            "Attribute");
    dumpDemarcation('-');

    SymbolTable *current_table = symbol_manager.getCurrentTable();
    for(auto &i: current_table->getEntries()){
        printf("%-33s", i.name.c_str());
        printf("%-11s", i.kind.c_str());
        printf("%d%-10s", i.level, (i.level == 0) ? "(global)" : "(local)");
        printf("%-17s", i.type.c_str());
        printf("%-11s\n", i.attr.c_str());
    }
    
    dumpDemarcation('-');
}

bool SemanticAnalyzer::checkRedeclaration(string name, uint32_t line_num, uint32_t col_num){
    vector<SymbolTable*> tables = symbol_manager.getTables();
    for(int i = 0;i<(int)tables.size();i++){
        for(auto &j: tables[i]->getEntries()){
            if(j.name == name and (j.kind == "loop_var" or i == (int)tables.size()-1)){
                dumpErrorMessage(line_num, col_num, "symbol '" + name + "' is redeclared");
                return true;
            }
        }
    }
    return false;
}

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    // push new symbol table
    symbol_manager.pushScope(new SymbolTable("void"));

    // get current symbol table
    SymbolTable *cur = symbol_manager.getCurrentTable();

    // insert symbol into current table
    cur->addSymbol(p_program.getNameCString(), "program", level, "void");

    // visit child nodes
    p_program.visitChildNodes(*this);

    // dump symbol table
    if(opt_sem) dumpSymbol();

    // pop symbol table
    symbol_manager.popScope();
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
    // check redeclaration
    if(checkRedeclaration(p_variable.getNameCString(), p_variable.getLocation().line, p_variable.getLocation().col)){
        return;
    }

    // get current symbol table
    SymbolTable *cur = symbol_manager.getCurrentTable();

    // insert symbol into current table
    if(p_variable.type == "constant"){
        cur->addSymbol(p_variable.getNameCString(), p_variable.type, level, p_variable.getTypeCString(), p_variable.getConstantValueNode()->getConstantValueCString());
    }
    else {
        cur->addSymbol(p_variable.getNameCString(), p_variable.type, level, p_variable.getTypeCString());
    }

    // check semantic error
    // invalid dimension
    if(!p_variable.hasValidDimension()){
        dumpErrorMessage(p_variable.getLocation().line,
                        p_variable.getLocation().col,
                        "'" + string(p_variable.getNameCString()) + "' declared as an array with an index that is not greater than 0");
        sem_err = true;
        cur->getLastSymbol()->valid = false;
    }
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    // check redeclaration
    if(!checkRedeclaration(p_function.getNameCString(), p_function.getLocation().line, p_function.getLocation().col)){
        // get current symbol table
        SymbolTable *cur = symbol_manager.getCurrentTable();

        // insert symbol into current table
        cur->addSymbol(p_function.getNameCString(), "function", level, p_function.getTypeCString(), p_function.getParametersTypeString());
    }

    // push new symbol table
    symbol_manager.pushScope(new SymbolTable(p_function.getTypeCString()));  

    // increase level
    level++;
    // this might be a nullptr!
    if(p_function.getBody()) p_function.getBody()->new_scope = false;
    for(auto &param: p_function.getParameters()){
        for(auto &var: param->getVariables()){
            var->setType("parameter");
        }
    }

    // visit child nodes
    p_function.visitChildNodes(*this);

    // dump symbol table
    if(opt_sem) dumpSymbol();

    // pop symbol table
    symbol_manager.popScope();

    // decrease level
    level--;
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    // push new symbol table if it is not from the function node
    if(p_compound_statement.new_scope){
        symbol_manager.pushScope(new SymbolTable(symbol_manager.getCurrentTable()->getReturnType()));

        // increase level
        level++;
    }

    // visit child nodes
    p_compound_statement.visitChildNodes(*this);

    if(p_compound_statement.new_scope){
        // dump symbol table
        if(opt_sem) dumpSymbol();

        // pop symbol table
        symbol_manager.popScope();

        // decrease level
        level--;
    }
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    // visit child nodes
    p_print.visitChildNodes(*this);

    // if child invalid -> don't dump error message
    if(!p_print.getTarget()->getIsValid()){
        return;
    }

    // check semantic error
    string type = p_print.getTarget()->getReturnType();

    // child is not scalar type
    if(!((type == "integer" or type == "real" or type == "boolean" or type == "string") and p_print.getTarget()->getDimension()==0)){
        dumpErrorMessage(p_print.getTarget()->getLocation().line,
                        p_print.getTarget()->getLocation().col,
                        "expression of print statement must be scalar type");
    }
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {

    // visit child nodes
    p_bin_op.visitChildNodes(*this);

    // check semantic error
    ExpressionNode *left = p_bin_op.getLeftOperand();
    ExpressionNode *right = p_bin_op.getRightOperand();
    string op = p_bin_op.getOpCString();
    string op_kind = "";
    if(op=="+" or op=="-" or op=="*" or op=="/"){
        op_kind = "arithmetic";
    }
    else if(op=="<" or op=="<=" or op==">" or op==">=" or op=="=" or op=="<>"){
        op_kind = "relational";
    }
    else if(op=="and" or op=="or"){
        op_kind = "boolean";
    }
    else if(op=="mod"){
        op_kind = "mod";
    }
    
    // children valid
    if((left->getIsValid() &&
        right->getIsValid())){
        // same dimension
        if(left->getDimension() == 0 && right->getDimension()==0){
            if(op_kind=="arithmetic"){
                if(op=="+" and left->getBaseType() == "string" and right->getBaseType() == "string"){
                    p_bin_op.setBaseType("string");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("string");
                }
                else if(left->getBaseType() == "integer" and right->getBaseType() == "integer"){
                    p_bin_op.setBaseType("integer");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("integer");
                }
                else if(left->getBaseType() == "real" and right->getBaseType() == "real"){
                    p_bin_op.setBaseType("real");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("real");
                }
                else if(left->getBaseType() == "integer" and right->getBaseType() == "real"){
                    p_bin_op.setBaseType("real");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("real");
                }
                else if(left->getBaseType() == "real" and right->getBaseType() == "integer"){
                    p_bin_op.setBaseType("real");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("real");
                }
                else{
                    dumpErrorMessage(p_bin_op.getLocation().line,
                            p_bin_op.getLocation().col,
                            "invalid operands to binary operator '" + 
                            string(p_bin_op.getOpCString()) + 
                            "' ('" +
                            left->getReturnType() +
                            "' and '" + 
                            right->getReturnType() +
                            "')");
                    p_bin_op.setIsValid(false);
                }
            }
            else if(op_kind=="relational"){
                if(left->getBaseType() == "integer" and right->getBaseType() == "integer"){
                    p_bin_op.setBaseType("boolean");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("boolean");
                }
                else if(left->getBaseType() == "real" and right->getBaseType() == "real"){
                    p_bin_op.setBaseType("boolean");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("boolean");
                }
                else if(left->getBaseType() == "integer" and right->getBaseType() == "real"){
                    p_bin_op.setBaseType("boolean");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("boolean");
                }
                else if(left->getBaseType() == "real" and right->getBaseType() == "integer"){
                    p_bin_op.setBaseType("boolean");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("boolean");
                }
                else{
                    dumpErrorMessage(p_bin_op.getLocation().line,
                            p_bin_op.getLocation().col,
                            "invalid operands to binary operator '" + 
                            string(p_bin_op.getOpCString()) + 
                            "' ('" +
                            left->getReturnType() +
                            "' and '" + 
                            right->getReturnType() +
                            "')");
                    p_bin_op.setIsValid(false);
                }
            }
            else if(op_kind=="boolean"){
                if(left->getBaseType() == "boolean" and right->getBaseType() == "boolean"){
                    p_bin_op.setBaseType("boolean");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("boolean");
                }
                else{
                    dumpErrorMessage(p_bin_op.getLocation().line,
                            p_bin_op.getLocation().col,
                            "invalid operands to binary operator '" + 
                            string(p_bin_op.getOpCString()) + 
                            "' ('" +
                            left->getReturnType() +
                            "' and '" + 
                            right->getReturnType() +
                            "')");
                    p_bin_op.setIsValid(false);
                }
            }
            else if(op_kind=="mod"){
                if(left->getBaseType() == "integer" and right->getBaseType() == "integer"){
                    p_bin_op.setBaseType("integer");
                    p_bin_op.setDimension(0);
                    p_bin_op.setReturnType("integer");
                }
                else{
                    dumpErrorMessage(p_bin_op.getLocation().line,
                            p_bin_op.getLocation().col,
                            "invalid operands to binary operator '" + 
                            string(p_bin_op.getOpCString()) + 
                            "' ('" +
                            left->getReturnType() +
                            "' and '" + 
                            right->getReturnType() +
                            "')");
                    p_bin_op.setIsValid(false);
                }
            }
        }
        // different dimension or reference to array
        else{
            dumpErrorMessage(p_bin_op.getLocation().line,
                            p_bin_op.getLocation().col,
                            "invalid operands to binary operator '" + 
                            string(p_bin_op.getOpCString()) + 
                            "' ('" +
                            left->getReturnType() +
                            "' and '" + 
                            right->getReturnType() +
                            "')");
            p_bin_op.setIsValid(false);
        }
    }    
    // children invalid -> don't dump error message
    else{
        p_bin_op.setIsValid(false);
    }
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    // visit child nodes
    p_un_op.visitChildNodes(*this);

    // check semantic error
    ExpressionNode *child = p_un_op.getOperand();
    string op = p_un_op.getOpCString();

    // child valid
    if(child->getIsValid()){
        // neg operator
        if(op=="neg" && child->getDimension()==0){
            if(child->getReturnType() == "integer"){
                p_un_op.setBaseType("integer");
                p_un_op.setDimension(0);
                p_un_op.setReturnType("integer");
            }
            else if(child->getReturnType() == "real"){
                p_un_op.setBaseType("real");
                p_un_op.setDimension(0);
                p_un_op.setReturnType("real");
            }
            else{
                dumpErrorMessage(p_un_op.getLocation().line,
                                p_un_op.getLocation().col,
                                "invalid operand to unary operator '" + 
                                string(p_un_op.getOpCString()) + 
                                "' ('" +
                                child->getReturnType() +
                                "')");
                p_un_op.setIsValid(false);
            }
        }
        // not operator
        else if(op=="not" && child->getDimension()==0){
            if(child->getBaseType() == "boolean"){
                p_un_op.setBaseType("boolean");
                p_un_op.setDimension(0);
                p_un_op.setReturnType("boolean");
            }
            else{
                dumpErrorMessage(p_un_op.getLocation().line,
                                p_un_op.getLocation().col,
                                "invalid operand to unary operator '" + 
                                string(p_un_op.getOpCString()) + 
                                "' ('" +
                                child->getReturnType() +
                                "')");
                p_un_op.setIsValid(false);
            }
        }
        else{
            dumpErrorMessage(p_un_op.getLocation().line,
                                p_un_op.getLocation().col,
                                "invalid operand to unary operator '" + 
                                string(p_un_op.getOpCString()) + 
                                "' ('" +
                                child->getReturnType() +
                                "')");
            p_un_op.setIsValid(false);
        }
    }
    // if child invalid -> don't dump error message
    else{
        p_un_op.setIsValid(false);
    }
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    // visit child nodes
    p_func_invocation.visitChildNodes(*this);

    // check semantic error
    vector<SymbolTable*> tables = symbol_manager.getTables();
    string func_name = string(p_func_invocation.getNameCString());
    for(int i = (int)tables.size()-1;i>=0;i--){
        for(auto &j: tables[i]->getEntries()){
            // symbol found
            if(j.name==func_name){
                // is function
                if(j.kind=="function"){
                    vector<string> params;
                    vector<string> args = p_func_invocation.getArgsType();

                    // get parameters
                    string param = j.attr;
                    size_t pos = 0;
                    string token;
                    string delimiter = ", ";
                    while((pos = param.find(delimiter)) != string::npos) {
                        token = param.substr(0, pos);
                        params.push_back(token);
                        param.erase(0, pos + delimiter.length());
                    }
                    if(param!="") params.push_back(param);

                    // parameter number match
                    if(params.size() == args.size()){
                        // check parameter type match
                        for(int i = 0;i<(int)params.size();i++){
                            // type mismatch and cannot coerce integer to real
                            if(params[i] != args[i] and !(params[i] == "real" and args[i] == "integer")){
                                pair<int, int> loc = p_func_invocation.getInvalidArgLocation(i);
                                dumpErrorMessage(loc.first,
                                loc.second,
                                "incompatible type passing '" + args[i] + "' to parameter of type '" + params[i] + "'");
                                p_func_invocation.setIsValid(false);
                                return;
                            }
                        }
                        // no semantic error
                        p_func_invocation.setBaseType(j.type.substr(0, j.type.find(" ")));
                        p_func_invocation.setDimension(j.dimension);
                        p_func_invocation.setReturnType(j.type);
                        return;
                    }
                    // parameter number mismatch
                    else{
                        dumpErrorMessage(p_func_invocation.getLocation().line,
                                p_func_invocation.getLocation().col,
                                "too few/much arguments provided for function '" + func_name + "'");
                        p_func_invocation.setIsValid(false);
                        return;
                    }
                }
                // is not function
                else{
                    dumpErrorMessage(p_func_invocation.getLocation().line,
                                p_func_invocation.getLocation().col,
                                "call of non-function symbol '" + func_name + "'");
                    p_func_invocation.setIsValid(false);
                    return;
                }
            }
        }
    }
    // symbol not found
    dumpErrorMessage(p_func_invocation.getLocation().line,
                                p_func_invocation.getLocation().col,
                                "use of undeclared symbol '" + func_name + "'");
    p_func_invocation.setIsValid(false);
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    // visit child nodes
    p_variable_ref.visitChildNodes(*this);

    // record invalid index
    bool has_valid_index = true;
    uint32_t invalid_index_line = 0;
    uint32_t invalid_index_col = 0;

    // child is invalid, skip semantic checking
    for(auto &expr : p_variable_ref.getIndices()){
        if(!expr->getIsValid()){
            p_variable_ref.setIsValid(false);
            return;
        }
    }

    for(auto &expr : p_variable_ref.getIndices()){
        if(expr->getReturnType() != "integer"){
            has_valid_index = false;
            invalid_index_line = expr->getLocation().line;
            invalid_index_col = expr->getLocation().col;
            break;
        }
    }

    // check semantic error
    vector<SymbolTable*> tables = symbol_manager.getTables();
    for(int i = (int)tables.size()-1;i>=0;i--){
        for(auto &j: tables[i]->getEntries()){
            // symbol found
            if(j.name == string(p_variable_ref.getNameCString())){
                // invalid, skip semantic checking
                if(!j.valid){
                    p_variable_ref.setIsValid(false);
                    return;
                }
                // type no error
                if(j.kind == "parameter" or j.kind == "variable" or j.kind == "loop_var" or j.kind == "constant"){
                    // index is integer
                    if(has_valid_index){
                        // array over subscript
                        if((int)p_variable_ref.getIndices().size()>j.dimension){
                            p_variable_ref.setIsValid(false);
                            dumpErrorMessage(p_variable_ref.getLocation().line,
                                p_variable_ref.getLocation().col,
                                "there is an over array subscript on '" + string(p_variable_ref.getNameCString()) + "'");
                            return;
                        }
                        // no semantic error
                        else{
                            string type = j.type;
                            int count = 0;
                            int start = -1;
                            int end = type.length();

                            // erase dimension
                            if((int)p_variable_ref.getIndices().size()!=0){
                                for(int ch = 0;ch<(int)type.length();ch++){
                                    if(type[ch] == '['){
                                        if(count == 0){
                                            start = ch;
                                        }
                                        count++;
                                    }
                                    else if(type[ch] == ']'){
                                        if(count == (int)p_variable_ref.getIndices().size()){
                                            end = ch+1;
                                            break;
                                        }
                                    }
                                }
                            }
                            
                            if(start != -1){
                                type.erase(type.begin()+start, type.begin()+end);
                            }
                            if(type.back() == ' '){
                                type.pop_back();
                            }
                            
                            p_variable_ref.setReturnType(type);
                            p_variable_ref.setBaseType(type.substr(0, type.find(" ")));
                            p_variable_ref.setDimension(j.dimension-count);
                            p_variable_ref.kind = j.kind;
                            p_variable_ref.table_index = i;
                            return;
                        }
                    }
                    // index is not integer
                    else{
                        p_variable_ref.setIsValid(false);
                        dumpErrorMessage(invalid_index_line,
                                invalid_index_col,
                                "index of array reference must be an integer");
                        return;
                    }
                }
                // type error
                else{
                    p_variable_ref.setIsValid(false);
                    dumpErrorMessage(p_variable_ref.getLocation().line,
                                p_variable_ref.getLocation().col,
                                "use of non-variable symbol '" + string(p_variable_ref.getNameCString()) + "'");
                    return;
                }
            }
        }
    }
    // undeclared variable
    p_variable_ref.setIsValid(false);
    dumpErrorMessage(p_variable_ref.getLocation().line,
                                p_variable_ref.getLocation().col,
                                "use of undeclared symbol '" + string(p_variable_ref.getNameCString()) + "'");
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    // visit child nodes
    p_assignment.visitChildNodes(*this);

    // if var ref invalid -> don't dump error message
    if(!p_assignment.getLvalueNode()->getIsValid()){
        return;
    }
    
    // check semantic error
    // variable reference is not array type
    if(p_assignment.getLvalueNode()->getDimension() == 0){
        // variable reference is not constant
        if(static_cast<VariableReferenceNode*>(p_assignment.getLvalueNode())->kind != "constant"){
            // variable reference is not loop variable or in the innermost scope (which means that it is the first assignment)
            if((static_cast<VariableReferenceNode*>(p_assignment.getLvalueNode())->kind == "loop_var" and symbol_manager.getCurrentTable()->isForScope()) or
               static_cast<VariableReferenceNode*>(p_assignment.getLvalueNode())->kind != "loop_var"){
                // if the assignment is the loop var, then it is already listed error as redeclaration, so return
                if(static_cast<VariableReferenceNode*>(p_assignment.getLvalueNode())->kind == "loop_var"){
                    return;
                }
                // if expression invalid -> don't dump error message
                if(!p_assignment.getExprNode()->getIsValid()){
                    return;
                }

                // expression is not array type
                if(p_assignment.getExprNode()->getDimension()==0){
                    // check whether var ref type is same as expr type
                    string type = p_assignment.getExprNode()->getReturnType();
                    string var_type = p_assignment.getLvalueNode()->getReturnType();
                    // type match or expr can be coerced into var ref
                    if(type == var_type or (type == "integer" and var_type == "real")){
                        return;
                    }
                    // type mismatch
                    else{
                        dumpErrorMessage(p_assignment.getLocation().line,
                                        p_assignment.getLocation().col,
                                        "assigning to '" + var_type + "' from incompatible type '" + type + "'");
                        return;
                    }
                }
                // expression is array type -> error
                else{
                    dumpErrorMessage(p_assignment.getExprNode()->getLocation().line,
                                    p_assignment.getExprNode()->getLocation().col,
                                    "array assignment is not allowed");
                    return;
                }
            }
            // variable reference is loop variable -> error
            else{
                dumpErrorMessage(p_assignment.getLvalueNode()->getLocation().line,
                                p_assignment.getLvalueNode()->getLocation().col,
                                "the value of loop variable cannot be modified inside the loop body");
                return;
            }
        }
        // variable reference is constant -> error
        else{
            dumpErrorMessage(p_assignment.getLvalueNode()->getLocation().line,
                            p_assignment.getLvalueNode()->getLocation().col,
                            "cannot assign to variable '" + string(p_assignment.getLvalueNode()->getNameCString()) + "' which is a constant");
            return;
        }
    }
    // variable reference is array type -> error
    else{
        dumpErrorMessage(p_assignment.getLvalueNode()->getLocation().line,
                        p_assignment.getLvalueNode()->getLocation().col,
                        "array assignment is not allowed");
        return;
    }
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    // visit child nodes
    p_read.visitChildNodes(*this);

    // if child invalid -> don't dump error message
    if(!p_read.getTarget()->getIsValid()){
        return;
    }

    // check semantic error
    string type = p_read.getTarget()->getReturnType();

    if(static_cast<VariableReferenceNode*>(p_read.getTarget())->kind == "constant" or
       static_cast<VariableReferenceNode*>(p_read.getTarget())->kind == "loop_var"){
        dumpErrorMessage(p_read.getTarget()->getLocation().line,
                        p_read.getTarget()->getLocation().col,
                        "variable reference of read statement cannot be a constant or loop variable");
    }
    // child is not scalar type
    else if(!((type == "integer" or type == "real" or type == "boolean" or type == "string") and p_read.getTarget()->getDimension()==0)){
        dumpErrorMessage(p_read.getTarget()->getLocation().line,
                        p_read.getTarget()->getLocation().col,
                        "variable reference of read statement must be scalar type");
    }
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    // visit child node
    p_if.visitChildNodes(*this);

    // check semantic error
    // return if condition invalid
    if(!p_if.getCondition()->getIsValid()){
        return;
    }

    // condition is not boolean type -> error
    if(!(p_if.getCondition()->getReturnType()=="boolean")){
        dumpErrorMessage(p_if.getCondition()->getLocation().line,
                        p_if.getCondition()->getLocation().col,
                        "the expression of condition must be boolean type");
    }
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    // visit child node
    p_while.visitChildNodes(*this);

    // check semantic error
    // return if condition invalid
    if(!p_while.getCondition()->getIsValid()){
        return;
    }

    // condition is not boolean type -> error
    if(!(p_while.getCondition()->getReturnType()=="boolean")){
        dumpErrorMessage(p_while.getCondition()->getLocation().line,
                        p_while.getCondition()->getLocation().col,
                        "the expression of condition must be boolean type");
    }
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    // push new symbol table
    SymbolTable* new_scope = new SymbolTable(symbol_manager.getCurrentTable()->getReturnType());
    new_scope->setForScope();
    symbol_manager.pushScope(new_scope);

    // increase level
    level++;

    // visit child nodes
    p_for.visitChildNodes(*this);

    // check semantic error
    pair<int, int> range = p_for.getLoopVarRange();
    if(range.first >= range.second){
        dumpErrorMessage(p_for.getLocation().line,
                        p_for.getLocation().col,
                        "the lower bound and upper bound of iteration count must be in the incremental order");
    }

    // dump symbol table
    if(opt_sem) dumpSymbol();

    // pop symbol table
    symbol_manager.popScope();

    // decrease level
    level--;
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    // visit child node
    p_return.visitChildNodes(*this);

    // check semantic error
    // get current table
    if(symbol_manager.getCurrentTable()->getReturnType() == "void"){
        dumpErrorMessage(p_return.getLocation().line,
                        p_return.getLocation().col,
                        "program/procedure should not return a value");
        return;
    }

    // stop checking if child invalid
    if(!p_return.getReturnValue()->getIsValid()){
        return;
    }

    // continue semantic checking
    string return_type = p_return.getReturnValue()->getReturnType();
    string specified_type = symbol_manager.getCurrentTable()->getReturnType();
    if(return_type == specified_type or (return_type == "integer" and specified_type == "real")){
        return;
    }
    else{
        dumpErrorMessage(p_return.getReturnValue()->getLocation().line,
                        p_return.getReturnValue()->getLocation().col,
                        "return '" +
                        return_type +
                        "' from a function with return type '" +
                        specified_type +
                        "'");
    }
}
