/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
using std::string;
class Symboltable {
public:
    string name;
    bool typeflag;
    int typeno;
    int lineno;
    int insertno;
};

class Node{
public:
    string name1;
    Node *next;
};

class Parser {
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();

    void parse_program();
    void parse_scope();
    void parse_scope_list();
    void parse_declaration();
    void parse_type_decl();
    string parse_type_name();
    void parse_var_decl();
    struct Node* parse_id_list();
    void parse_stmt_list();
    void parse_stmt();
    void parse_assign_stmt();
    void parse_while_stmt();
    int parse_expr();
    int parse_term();
    int parse_factor();
    void parse_condition();
    int parse_primary();
    void parse_relop();

  public:
    void ParseInput();
};

#endif

