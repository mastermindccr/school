#ifndef SEMA_SEMANTIC_ANALYZER_H
#define SEMA_SEMANTIC_ANALYZER_H

#include "visitor/AstNodeVisitor.hpp"
#include <bits/stdc++.h>
using namespace std;

class SymbolEntry {
  public:
    SymbolEntry(string name, string kind, int level, string type, string attr){
        this->name = name;
        this->kind = kind;
        this->level = level;
        this->type = type;
        this->attr = attr;
        for(int i = 0; i < (int)type.length(); i++){
            if(type[i] == '['){
                this->dimension++;
            }
        }
    }
    SymbolEntry(string name, string kind, int level, string type){
        this->name = name;
        this->kind = kind;
        this->level = level;
        this->type = type;
        for(int i = 0; i < (int)type.length(); i++){
            if(type[i] == '['){
                this->dimension++;
            }
        }
    }
    string name;
    string kind;
    int level;
    string type;
    string attr;
    int dimension = 0;
    bool valid = true;
};

class SymbolTable {
  public:
    SymbolTable(string ret_type){
        this->ret_type = ret_type;
    }
    void addSymbol(string name, string kind, int level, string type){
        entries.push_back(SymbolEntry(name, kind, level, type));
    }
    void addSymbol(string name, string kind, int level, string type, string attr){
        entries.push_back(SymbolEntry(name, kind, level, type, attr));
    }
    vector<SymbolEntry>& getEntries(){
        return entries;
    }
    SymbolEntry* getLastSymbol(){
        return &entries.back();
    }
    string getReturnType(){
        return ret_type;
    }
    void setForScope(){
        is_for_scope = true;
    }
    bool isForScope(){
        return is_for_scope;
    }
    
  private:
    std::string ret_type;
    bool is_for_scope = false;
    std::vector<SymbolEntry> entries;
};

class SymbolManager {
  public:
    void pushScope(SymbolTable *new_scope){
        tables.push_back(new_scope);
    }
    void popScope(){
        tables.pop_back();
    }
    SymbolTable* getCurrentTable(){
        return tables.back();
    }
    vector<SymbolTable*>& getTables(){
        return tables;
    }
  private:
    vector<SymbolTable*> tables;
};

class SemanticAnalyzer final : public AstNodeVisitor {
  private:
    int level = 0;
    SymbolManager symbol_manager;
    // TODO: something like symbol manager (manage symbol tables)
    //       context manager, return type manager

  public:
    ~SemanticAnalyzer() = default;
    SemanticAnalyzer() = default;

    void visit(ProgramNode &p_program) override;
    void visit(DeclNode &p_decl) override;
    void visit(VariableNode &p_variable) override;
    void visit(ConstantValueNode &p_constant_value) override;
    void visit(FunctionNode &p_function) override;
    void visit(CompoundStatementNode &p_compound_statement) override;
    void visit(PrintNode &p_print) override;
    void visit(BinaryOperatorNode &p_bin_op) override;
    void visit(UnaryOperatorNode &p_un_op) override;
    void visit(FunctionInvocationNode &p_func_invocation) override;
    void visit(VariableReferenceNode &p_variable_ref) override;
    void visit(AssignmentNode &p_assignment) override;
    void visit(ReadNode &p_read) override;
    void visit(IfNode &p_if) override;
    void visit(WhileNode &p_while) override;
    void visit(ForNode &p_for) override;
    void visit(ReturnNode &p_return) override;

    void dumpDemarcation(const char chr);
    void dumpSymbol();
    bool checkRedeclaration(string name, uint32_t line, uint32_t col);
};

#endif
