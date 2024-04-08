#include "codegen/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <iostream>
#include <cstring>
using namespace std;

CodeGenerator::CodeGenerator(const std::string &source_file_name,
                             const std::string &save_path,
                             const SymbolManager *const p_symbol_manager)
    : m_symbol_manager_ptr(p_symbol_manager),
      m_source_file_path(source_file_name) {
    // FIXME: assume that the source file is always xxxx.p
    const auto &real_path =
        save_path.empty() ? std::string{"."} : save_path;
    auto slash_pos = source_file_name.rfind("/");
    auto dot_pos = source_file_name.rfind(".");

    if (slash_pos != std::string::npos) {
        ++slash_pos;
    } else {
        slash_pos = 0;
    }
    auto output_file_path{
        real_path + "/" +
        source_file_name.substr(slash_pos, dot_pos - slash_pos) + ".S"};
    m_output_file.reset(fopen(output_file_path.c_str(), "w"));
    assert(m_output_file.get() && "Failed to open output file");
}

static void dumpInstructions(FILE *p_out_file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(p_out_file, format, args);
    va_end(args);
}

void CodeGenerator::visit(ProgramNode &p_program) {
    constexpr const char *const riscv_assembly_file_prologue =
        "    .file \"%s\"\n"
        "    .option nopic\n";
    
    dumpInstructions(m_output_file.get(), riscv_assembly_file_prologue,
                     m_source_file_path.c_str());

    // Reconstruct the hash table for looking up the symbol entry
    // Hint: Use symbol_manager->lookup(symbol_name) to get the symbol entry.
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_program.getSymbolTable());

    vector<string> global_var;
    vector<string> global_const;

    for(auto &decls: p_program.getDeclNodes()){
        for(auto &vars: decls->getVariables()){
            string name = m_symbol_manager_ptr->lookup(vars->getName())->getName();
            int kind = int(m_symbol_manager_ptr->lookup(vars->getName())->getKind());
            if(kind==3){ // global variable
                global_var.emplace_back(name);
            }
            else{ // global constant
                global_const.emplace_back(name);
            }
        }
    }

    if(!global_var.empty()){
        dumpInstructions(m_output_file.get(), ".section    .bss\n");
        for(auto &var: global_var){
            dumpInstructions(m_output_file.get(), "    .comm %s, 4, 4\n", var.c_str());
        }
    }
    
    if(!global_const.empty()){
        dumpInstructions(m_output_file.get(), ".section    .rodata\n");
        for(auto &constt: global_const){
            dumpInstructions(m_output_file.get(), "    .align 2\n");
            dumpInstructions(m_output_file.get(), "    .globl %s\n", constt.c_str());
            dumpInstructions(m_output_file.get(), "    .type %s, @object\n", constt.c_str());
            dumpInstructions(m_output_file.get(), "%s:\n", constt.c_str());
            dumpInstructions(m_output_file.get(), "    .word %d\n", m_symbol_manager_ptr->lookup(constt)->getAttribute().constant()->integer());
        }
    }

    // there are functions
    if(!p_program.getFuncNodes().empty()){
        dumpInstructions(m_output_file.get(), ".section    .text\n");
    }

    auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(*this); };
    
    for_each(p_program.getDeclNodes().begin(), p_program.getDeclNodes().end(),
             visit_ast_node);

    for_each(p_program.getFuncNodes().begin(), p_program.getFuncNodes().end(),
             visit_ast_node);


    // main prologue
    constexpr const char *const main_prologue = 
        ".section    .text\n"
        "    .align 2\n"
        "    .globl main\n" // emit symbol 'main' to the global symbol table
        "    .type main, @function\n"
        "main:\n"
        "    addi sp, sp, -128\n"    // move stack pointer to lower address to allocate a new stack
        "    sw ra, 124(sp)\n"       // save return address of the caller function in the current stack
        "    sw s0, 120(sp)\n"       // save frame pointer of the last stack in the current stack
        "    addi s0, sp, 128\n";    // move frame pointer to the bottom of the current stack
    dumpInstructions(m_output_file.get(), main_prologue);

    const_cast<CompoundStatementNode &>(p_program.getBody()).accept(*this);

    // main eiplogue
    constexpr const char *const main_epilogue = 
        "    lw ra, 124(sp)\n"
        "    lw s0, 120(sp)\n"
        "    addi sp, sp, 128\n"
        "    jr ra\n"
        "    .size main, .-main\n";
    dumpInstructions(m_output_file.get(), main_epilogue);

    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_program.getSymbolTable());
}

void CodeGenerator::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void CodeGenerator::visit(VariableNode &p_variable) {
    const SymbolEntry* entry = m_symbol_manager_ptr->lookup(p_variable.getName());
    int kind = int(entry->getKind());
    if(kind==2){ // parameter
        // calculate this is the i'th parameter
        int offset = entry->offset;
        int arg_num = (-12 - offset)/4;
        dumpInstructions(m_output_file.get(), "    sw a%d, %d(s0)\n", arg_num, offset);
    }
    else if(kind==5){ // local constant
        dumpInstructions(m_output_file.get(), "    li t0, %d\n", entry->getAttribute().constant()->integer());
        dumpInstructions(m_output_file.get(), "    sw t0, %d(s0)\n", entry->offset);
    }
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
    dumpInstructions(m_output_file.get(), "    li t0, %s\n", p_constant_value.getConstantValueCString());
    dumpInstructions(m_output_file.get(), "    sw t0, 0(sp)\n");
}

void CodeGenerator::visit(FunctionNode &p_function) {
    dumpInstructions(m_output_file.get(), "    .align 2\n");
    dumpInstructions(m_output_file.get(), "    .globl %s\n", p_function.getNameCString());
    dumpInstructions(m_output_file.get(), "    .type %s, @function\n", p_function.getNameCString());
    dumpInstructions(m_output_file.get(), "%s:\n", p_function.getNameCString());

    // add function prologue
    constexpr const char *const function_prologue =
        "    addi sp, sp, -128\n"    // move stack pointer to lower address to allocate a new stack
        "    sw ra, 124(sp)\n"       // save return address of the caller function in the current stack
        "    sw s0, 120(sp)\n"       // save frame pointer of the last stack in the current stack
        "    addi s0, sp, 128\n";    // move frame pointer to the bottom of the current stack
    
    dumpInstructions(m_output_file.get(), function_prologue);

    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_function.getSymbolTable());

    p_function.visitChildNodes(*this);

    // add function epilogue
    constexpr const char *const function_epilogue =
        "    lw ra, 124(sp)\n"    // load return address saved in the current stack
        "    lw s0, 120(sp)\n"    // move frame pointer back to the bottom of the last stack
        "    addi sp, sp, 128\n"  // move stack pointer back to the top of the last stack
        "    jr ra\n"             // jump back to the caller function
        "    .size %s, .-%s\n";   
    
    dumpInstructions(m_output_file.get(), function_epilogue,
                     p_function.getNameCString(), p_function.getNameCString());

    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_compound_statement.getSymbolTable());

    p_compound_statement.visitChildNodes(*this);

    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(
        p_compound_statement.getSymbolTable());
}

void CodeGenerator::visit(PrintNode &p_print) {
    p_print.visitChildNodes(*this);
    dumpInstructions(m_output_file.get(), "    lw a0, 0(sp)\n");
    dumpInstructions(m_output_file.get(), "    addi sp, sp, 4\n");
    dumpInstructions(m_output_file.get(), "    jal ra, printInt\n"); // call printInt function
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    p_bin_op.visitChildNodes(*this);

    // retrieve left operand and right operand from the stack
    dumpInstructions(m_output_file.get(), "    lw t1, 0(sp)\n");
    dumpInstructions(m_output_file.get(), "    lw t0, 4(sp)\n");
    dumpInstructions(m_output_file.get(), "    addi sp, sp, 8\n");

    if(!strcmp(p_bin_op.getOpCString(), "+")){
        dumpInstructions(m_output_file.get(), "    add t2, t0, t1\n");
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t2, 0(sp)\n");
    }
    else if(!strcmp(p_bin_op.getOpCString(), "-")){
        dumpInstructions(m_output_file.get(), "    sub t2, t0, t1\n");
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t2, 0(sp)\n");
    }
    else if(!strcmp(p_bin_op.getOpCString(), "*")){
        dumpInstructions(m_output_file.get(), "    mul t2, t0, t1\n");
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t2, 0(sp)\n");
    }
    else if(!strcmp(p_bin_op.getOpCString(), "/")){
        dumpInstructions(m_output_file.get(), "    div t2, t0, t1\n");
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t2, 0(sp)\n");
    }
    else if(!strcmp(p_bin_op.getOpCString(), "mod")){
        dumpInstructions(m_output_file.get(), "    rem t2, t0, t1\n");
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t2, 0(sp)\n");
    }
    else if(!strcmp(p_bin_op.getOpCString(), "<")){
        dumpInstructions(m_output_file.get(), "    bge t0, t1, L%d\n", label_count+1);
    }
    else if(!strcmp(p_bin_op.getOpCString(), "<=")){
        dumpInstructions(m_output_file.get(), "    bgt t0, t1, L%d\n", label_count+1);
    }
    else if(!strcmp(p_bin_op.getOpCString(), ">")){
        dumpInstructions(m_output_file.get(), "    ble t0, t1, L%d\n", label_count+1);
    }
    else if(!strcmp(p_bin_op.getOpCString(), ">=")){
        dumpInstructions(m_output_file.get(), "    blt t0, t1, L%d\n", label_count+1);
    }
    else if(!strcmp(p_bin_op.getOpCString(), "=")){
        dumpInstructions(m_output_file.get(), "    bne t0, t1, L%d\n", label_count+1);
    }
    else if(!strcmp(p_bin_op.getOpCString(), "<>")){
        dumpInstructions(m_output_file.get(), "    beq t0, t1, L%d\n", label_count+1);
    }
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    p_un_op.visitChildNodes(*this);

    // retrieve operand from the stack
    dumpInstructions(m_output_file.get(), "    lw t0, 0(sp)\n");
    dumpInstructions(m_output_file.get(), "    addi sp, sp, 4\n");

    if(!strcmp(p_un_op.getOpCString(), "neg")){
        dumpInstructions(m_output_file.get(), "    neg t1, t0\n");
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t1, 0(sp)\n");
    }
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    p_func_invocation.visitChildNodes(*this);
    
    // retrieve arguments from the stack
    int num_args = p_func_invocation.getArguments().size();
    for(int i=num_args-1; i>=0; i--){
        dumpInstructions(m_output_file.get(), "    lw a%d, 0(sp)\n", i);
        dumpInstructions(m_output_file.get(), "    addi sp, sp, 4\n");
    }
    dumpInstructions(m_output_file.get(), "    jal ra, %s\n", p_func_invocation.getNameCString());
    dumpInstructions(m_output_file.get(), "    mv t0, a0\n"); // move the return value to a register
    dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
    dumpInstructions(m_output_file.get(), "    sw t0, 0(sp)\n"); // save the return value to the stack
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    // save value only

    auto entry = m_symbol_manager_ptr->lookup(p_variable_ref.getName());
    int level = entry->getLevel();
    if(level==0){ // global
        dumpInstructions(m_output_file.get(), "    la t0, %s\n", p_variable_ref.getNameCString());
        dumpInstructions(m_output_file.get(), "    lw t1, 0(t0)\n", p_variable_ref.getNameCString());
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t1, 0(sp)\n");
    }
    else{ // local
        dumpInstructions(m_output_file.get(), "    lw t0, %d(s0)\n", entry->offset);
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t0, 0(sp)\n");
    }
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    // special case: variable reference at the lhs of assignment node should be saved in pointer
    string var_name = p_assignment.getLvalue().getName();
    const SymbolEntry *entry = m_symbol_manager_ptr->lookup(var_name);
    int level = entry->getLevel();
    if(level==0){ // global
        dumpInstructions(m_output_file.get(), "    la t0, %s\n", var_name.c_str());
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t0, 0(sp)\n");
    }
    else{ // local
        dumpInstructions(m_output_file.get(), "    addi t0, s0, %d\n", entry->offset);
        dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
        dumpInstructions(m_output_file.get(), "    sw t0, 0(sp)\n");
    }

    const_cast<ExpressionNode &>(p_assignment.getExpr()).accept(*this);

    // retrieve variable reference and expression value from the stack
    dumpInstructions(m_output_file.get(), "    lw t0, 0(sp)\n");
    dumpInstructions(m_output_file.get(), "    lw t1, 4(sp)\n");
    dumpInstructions(m_output_file.get(), "    addi sp, sp, 8\n");
    dumpInstructions(m_output_file.get(), "    sw t0, 0(t1)\n");
    
}

void CodeGenerator::visit(ReadNode &p_read) {
    const char* var_name = p_read.getTarget().getNameCString();
    dumpInstructions(m_output_file.get(), "    la t0, %s\n", var_name);
    dumpInstructions(m_output_file.get(), "    addi sp, sp, -4\n");
    dumpInstructions(m_output_file.get(), "    sw t0, 0(sp)\n");
    dumpInstructions(m_output_file.get(), "    jal ra, readInt\n");
    dumpInstructions(m_output_file.get(), "    lw t0, 0(sp)\n");
    dumpInstructions(m_output_file.get(), "    addi sp, sp, 4\n");
    dumpInstructions(m_output_file.get(), "    sw a0, 0(t0)\n");
}

void CodeGenerator::visit(IfNode &p_if) {
    const_cast<ExpressionNode &>(p_if.getCondition()).accept(*this);

    int label = label_count;
    label_count += 3;

    // set label && run body
    dumpInstructions(m_output_file.get(), "L%d:\n", label);
    const_cast<CompoundStatementNode &>(p_if.getBody()).accept(*this);
    dumpInstructions(m_output_file.get(), "    j L%d\n", label+2); // escape else body

    // set label && run else body
    
    dumpInstructions(m_output_file.get(), "L%d:\n", label+1);
    const_cast<CompoundStatementNode &>(p_if.getElseBody()).accept(*this);

    // set label representing end of if
    dumpInstructions(m_output_file.get(), "L%d:\n", label+2);
}

void CodeGenerator::visit(WhileNode &p_while) {
    int label = label_count;
    label_count ++; // move the offset so that the jumped label is correct in binary operator

    // set label && test condition
    dumpInstructions(m_output_file.get(), "L%d:\n", label);
    const_cast<ExpressionNode &>(p_while.getCondition()).accept(*this);

    label_count += 2; // move the offset to the correct position

    // if condition is true, enter while body
    dumpInstructions(m_output_file.get(), "L%d:\n", label+1);
    const_cast<CompoundStatementNode &>(p_while.getBody()).accept(*this);
    dumpInstructions(m_output_file.get(), "    j L%d\n", label); // jump to the beginning of while

    // set label representing end of while
    dumpInstructions(m_output_file.get(), "L%d:\n", label+2);
}

void CodeGenerator::visit(ForNode &p_for) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_for.getSymbolTable());

    const_cast<DeclNode &>(p_for.getLoopVarDecl()).accept(*this);
    const_cast<AssignmentNode &>(p_for.getInitStmt()).accept(*this);

    // get current label
    int label = label_count;
    label_count += 3;

    // check whether break loop
    dumpInstructions(m_output_file.get(), "L%d:\n", label);
    const SymbolEntry* entry = m_symbol_manager_ptr->lookup(const_cast<DeclNode &>(p_for.getLoopVarDecl()).getVariables()[0]->getName());
    dumpInstructions(m_output_file.get(), "    lw t0, %d(s0)\n", entry->offset);
    dumpInstructions(m_output_file.get(), "    li t1, %s\n", p_for.getUpperBound().getConstantValueCString());
    dumpInstructions(m_output_file.get(), "    bge t0, t1, L%d\n", label+2);

    // get into for body
    dumpInstructions(m_output_file.get(), "L%d:\n", label+1);
    const_cast<CompoundStatementNode &>(p_for.getBody()).accept(*this);
    dumpInstructions(m_output_file.get(), "    lw t0, %d(s0)\n", entry->offset);
    dumpInstructions(m_output_file.get(), "    addi t0, t0, 1\n");
    dumpInstructions(m_output_file.get(), "    sw t0, %d(s0)\n", entry->offset);
    dumpInstructions(m_output_file.get(), "    j L%d\n", label);

    // exit for loop
    dumpInstructions(m_output_file.get(), "L%d:\n", label+2);

    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_for.getSymbolTable());
}

void CodeGenerator::visit(ReturnNode &p_return) {
    const_cast<ExpressionNode &>(p_return.getReturnValue()).accept(*this);
    dumpInstructions(m_output_file.get(), "    lw a0, 0(sp)\n");
    dumpInstructions(m_output_file.get(), "    addi sp, sp, 4\n");
}
