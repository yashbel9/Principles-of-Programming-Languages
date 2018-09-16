/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of 
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;
struct usee{
    string n2;
    int useno;
};
struct decl{
    string n;
    int declno;
};
vector<struct decl> fordecl;
vector<struct usee> foruse;
vector<struct Symboltable> tnvtable;
int tnvcount = 0, inscount = 0;
enum mytypes {INTTYPE = 0, REALTYPE, BOOLTYPE, STRINGTYPE, LONGTYPE};
string printerror = "", getid = "";
bool eflag = false;
void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}
std::string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

Symboltable* repeated(string name)
{
    vector<Symboltable> :: iterator i = tnvtable.begin();
    while(i != tnvtable.end())
    {
        if(i->name == name)
        {
            return &(*i);
        }
        i++;
    }
    return NULL;
}

int insertnode(string name, bool typeflag)
{
    Symboltable *st = repeated(name);
    if(st != NULL)
    {
        return st -> typeno;
    }
    Symboltable temp;
    temp.name = name;
    temp.typeflag = typeflag;
    temp.typeno = tnvcount++;
    temp.insertno = inscount++;
    tnvtable.push_back(temp);
    return temp.typeno;
}


int asstype(int t1, int t2)
{
    int lasttype;
    if(t1==-1)
        t1=tnvcount;
    if(t2==-1)
        t2=tnvcount;
    if(t1>=5 && t2<5)
    {
        lasttype = t2;
    }
    else if(t1>=5 && t2>=5)
    {
        lasttype = t1;
    }
    else if(t1<5 && t2<5)
    {
        if(t1==t2)
        {
            lasttype = t2;
        }
        else
        {
            lasttype = -1;
        }
    }
    for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
    {
        if(i->typeno == t1 || i->typeno == t2)
        {
            i->typeno = lasttype;
        }
    }
    return lasttype;
}
// Parsing

void Parser::parse_program()
{
    // program -> scope
    
    parse_scope();
}

void Parser::parse_scope()
{
    // scope -> { scope_list }
    
    expect(LBRACE);
    parse_scope_list();
    expect(RBRACE);
}


void Parser::parse_scope_list()
{
    // scope_list -> stmt
    // scope_list -> scope
    // scope_list -> declaration
    // scope_list -> stmt scope_list
    // scope_list -> scope scope_list
    // scope_list -> declaration scope_list

    // TODO
    Token t = peek();
    if(t.token_type == ID || t.token_type == WHILE)
    {
        //lexer.UngetToken(t);
        parse_stmt();
        Token t1 = peek();
        if(t1.token_type == ID || t1.token_type == WHILE || t1.token_type == TYPE || t1.token_type == VAR || t1.token_type == LBRACE)
        {
            parse_scope_list();
        }
        else if(t1.token_type == RBRACE)
        {
            
        }

    }
    else if(t.token_type == LBRACE)
    {
        //lexer.UngetToken(t);
        parse_scope();
        Token t1 = peek();
        if(t1.token_type == ID || t1.token_type == WHILE || t1.token_type == TYPE || t1.token_type == VAR || t1.token_type == LBRACE)
        {
            parse_scope_list();
        }
        else if(t1.token_type == RBRACE)
        {
            
        }

    }
    else if(t.token_type == TYPE || t.token_type == VAR)
    {
        //lexer.UngetToken(t);
        parse_declaration();
        Token t1 = peek();
        if(t1.token_type == ID || t1.token_type == WHILE || t1.token_type == TYPE || t1.token_type == VAR || t1.token_type == LBRACE)
        {
            parse_scope_list();
        }
        else if(t1.token_type == RBRACE)
        {
            
        }
    }
    else
    {
        syntax_error();
    }
}

void Parser::parse_declaration()
{
    // declaration -> type_decl
    // declaration -> var_decl

    Token t = peek();
    if (t.token_type == TYPE)
    	parse_type_decl();
    if (t.token_type == VAR)
    	parse_var_decl();
}

void Parser::parse_type_decl()
{
    // type_decl -> TYPE id_list COLON type_name SEMICOLON

    expect(TYPE);
    Node* start = parse_id_list();
    expect(COLON);
    string tname = parse_type_name();
    expect(SEMICOLON);
    int tno = -1;
    for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
    {
        if(i->name == tname)
        {
            tno = i-> typeno;
            break;
        }
    }
    if(tno == -1)
        tno = tnvcount;
    while(start!=NULL)
    {
        Symboltable temp;
        temp.name = start->name1;
        temp.typeflag = true;
        temp.typeno = tno;
        temp.insertno = inscount++;
        Symboltable* temp1 = repeated(temp.name);
        if(temp1 == NULL)
        {
            tnvtable.push_back(temp);
        }
        else
        {
            
        }
        start = start->next;
    }
    insertnode(tname, true);
}

string Parser::parse_type_name()
{
    // type_name -> REAL
    // type_name -> INT
    // type_name -> BOOLEAN
    // type_name -> STRING
    // type_name -> LONG
    // type_name -> ID

    // TODO
    Token t = lexer.GetToken();
    if(t.token_type == REAL)
    {
        return "REAL";
    }
    else if(t.token_type == INT)
    {
        return "INT";
    }
    else if(t.token_type == BOOLEAN)
    {
        return "BOOLEAN";
    }
    else if(t.token_type == STRING)
    {
        return "STRING";
    }
    else if(t.token_type == LONG)
    {
        return "LONG";
    }
    else if(t.token_type == ID)
    {
        /*decl tem;
        tem.n = t.lexeme;
        tem.declno = t.line_no;
        fordecl.push_back(tem);*/
        return t.lexeme;
    }
    else
    {
        
        syntax_error();
    }
}

void Parser::parse_var_decl()
{
    // var_decl -> VAR id_list COLON type_name SEMICOLON
    expect(VAR);
    Node* start = parse_id_list();
    expect(COLON);
    string tname = parse_type_name();
    for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
    {
        if(i->name == tname && i->typeflag==false && eflag==false)
        {
            printerror = "ERROR CODE 2.3 "+i->name;
            eflag=true;
        }
    }
    expect(SEMICOLON);
    int tno = -1;
    for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
    {
        if(i->name == tname)
        {
            tno = i->typeno;
            break;
        }
    }
    if(tno == -1)
        tno = tnvcount;
    while(start!=NULL)
    {
        Symboltable temp;
        temp.name = start->name1;
        temp.typeflag = false;
        temp.typeno = tno;
        temp.insertno = inscount++;
        Symboltable* temp1 = repeated(temp.name);
        if(temp1 == NULL)
        {
            tnvtable.push_back(temp);
        }
        else
        {
            if(temp1->typeflag==true && eflag == false)
            {
                printerror="ERROR CODE 1.2 "+temp1->name;
                eflag = true;
            }
            else if(temp1->typeflag==false && eflag==false)
            {
                printerror="ERROR CODE 2.1 "+temp1->name;
                eflag = true;
            }
        }
        start = start->next;
    }
    insertnode(tname, true);
}

Node* Parser::parse_id_list()
{
    // id_list -> ID
    // id_list -> ID COMMA id_list

    // TODO
    Token t1 = expect(ID);
    decl tem;
    tem.n = t1.lexeme;
    tem.declno = t1.line_no;
    fordecl.push_back(tem);
    Node *list;
    list = new Node();
    list -> name1 = t1.lexeme;
    list -> next = NULL;
    Token t = lexer.GetToken();
    if(t.token_type == COMMA)
    {
        list->next=parse_id_list(); 
        return list;
    }
    else if(t.token_type == COLON)
    {
        lexer.UngetToken(t);
        return list;
    }
    else 
    {
        syntax_error();    
    }
}

void Parser::parse_stmt_list()
{
    // stmt_list -> stmt
    // stmt_list -> stmt stmt_list
    
    parse_stmt();
    Token t = peek();
    if (t.token_type == WHILE || t.token_type == ID)
    {
        // stmt_list -> stmt stmt_list
        parse_stmt_list();
    }
    else if (t.token_type == RBRACE)
    {
        // stmt_list -> stmt
    }
    else
    {
        syntax_error();
    }
}

void Parser::parse_stmt()
{
    // stmt -> assign_stmt
    // stmt -> while_stmt
 
    // TODO
    Token t = peek();
    if(t.token_type == ID)
    {
        parse_assign_stmt();
    }
    else if(t.token_type == WHILE)
    {
        parse_while_stmt();    
    }
    else
    {
        syntax_error();    
    }
    
}

void Parser::parse_assign_stmt()
{
    // assign_stmt -> ID EQUAL expr SEMICOLON

    // TODO
    Token t = expect(ID);
    usee ne;
    ne.n2 = t.lexeme;
    ne.useno = t.line_no;
    foruse.push_back(ne);
    Token t1 = expect(EQUAL);
    int exprtype = parse_expr();
    Token t2 = expect(SEMICOLON);
    int tno = -1;
    for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
    {
        if(i->name == t.lexeme)
        {
            tno = i->typeno;
            break;
        }
    }
    if(tno == -1)
    {
        tno = insertnode(t.lexeme, false);
    }
    if(exprtype == -1)
    {
        exprtype = insertnode(getid, false);
    }
    int newtype = asstype(tno, exprtype);
    if(newtype == -1 && eflag==false)
    {
        printerror = "TYPE MISMATCH "+to_string(t.line_no)+" "+"C1";
        eflag = true;
    }
}

void Parser::parse_while_stmt()
{
   // while_stmt -> WHILE condition LBRACE stmt list RBRACE

    // TODO
    expect(WHILE);
    parse_condition();
    expect(LBRACE);
    parse_stmt_list();
    expect(RBRACE);
}

int Parser::parse_expr()
{
    // expr -> term 
    // expr -> term + expr

    // TODO
    int termtype = parse_term();
    Token t = lexer.GetToken();
    if(t.token_type == PLUS)
    {
        //lexer.GetToken();
        
        int exprtype = parse_expr();
        if(exprtype == -1)
        {
            exprtype = insertnode(getid, false);
        }
        int newtype = asstype(termtype, exprtype);
        if(newtype == -1 && eflag==false)
        {
            printerror = "TYPE MISMATCH "+to_string(t.line_no)+" "+"C2";
            eflag = true;
        }
        else
        {
            return newtype;
        }
    }
    else if(t.token_type == SEMICOLON || t.token_type == RPAREN)
    {
        lexer.UngetToken(t);
        return termtype;
    }
    else
    {
        syntax_error();
    }
    
}

int Parser::parse_term()
{
    // term -> factor
    // term -> factor MULT term

    // TODO
    int factortype = parse_factor();
    Token t = lexer.GetToken();
    if(t.token_type == MULT)
    {
        //lexer.GetToken();
        int termtype = parse_term();
        if(termtype == -1)
        {
            termtype = tnvcount;
        }
        int newtype = asstype(factortype, termtype);
        if(newtype == -1 && eflag==false)
        {
            printerror = "TYPE MISMATCH "+to_string(t.line_no)+" "+"C2";
            eflag = true;
        }
        else
        {
            return newtype;
        }
    }
    else if(t.token_type == PLUS || t.token_type == SEMICOLON || t.token_type == RPAREN)
    {
        lexer.UngetToken(t);
        return factortype;
    }
    else
    {
        syntax_error();    
    }
}

int Parser::parse_factor()
{
    // factor -> LPAREN expr RPAREN
    // factor -> NUM
    // factor -> REALNUM
    // factor -> ID

    // TODO
    Token t = lexer.GetToken();
    if(t.token_type == LPAREN)
    {
        int exprtype = parse_expr();
        if(exprtype == -1)
            exprtype = insertnode(getid, false);
        Token t1 = lexer.GetToken();
        if(t1.token_type == RPAREN)
        {
            return exprtype;
        }
        else
        {
            syntax_error();
        }
    }
    else if(t.token_type == NUM)
    {
        return INTTYPE;
    }
    else if(t.token_type == REALNUM)
    {
        return REALTYPE;
    }
    else if(t.token_type == ID)
    {
        usee ne;
        ne.n2 = t.lexeme;
        ne.useno = t.line_no;
        foruse.push_back(ne);
        for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
        {
            if(i->name == t.lexeme && i->typeflag == true && eflag==false)
            {
                printerror = "ERROR CODE 1.3 "+t.lexeme;
                eflag=true;
            }
        }
        getid = t.lexeme;
        int tno = -1;
        for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
        {
            if(i->name == t.lexeme)
            {
                tno = i->typeno;
                break;
            }
        }
        if(tno == -1)
            tno = tnvcount;
        return tno;
    }
    else
    {
        syntax_error();
    }
}

void Parser::parse_condition()
{
    // condition -> ID
    // condition -> primary relop primary

    Token t = lexer.GetToken();
    if(t.token_type == NUM || t.token_type == REALNUM)
    {
        lexer.UngetToken(t);
        int primarytype = parse_primary();
        parse_relop();
        int primarytype2 = parse_primary();
        if(primarytype == -1)
        {
            primarytype = insertnode(getid, false);
        }
        if(primarytype2 == -1)
        {
            primarytype2 = insertnode(getid, false);
        }
        int newtype = asstype(primarytype, primarytype2);
        if(newtype == -1 && eflag==false)
        {
            printerror = "TYPE MISMATCH "+to_string(t.line_no)+" "+"C3";
            eflag = true;
        }
    }
    else if(t.token_type == ID)
    {
        usee ne;
        ne.n2 = t.lexeme;
        ne.useno = t.line_no;
        foruse.push_back(ne);
        Token t1 = peek();
        if(t1.token_type == GREATER || t1.token_type == GTEQ || t1.token_type == LESS || t1.token_type == NOTEQUAL || t1.token_type == LTEQ)
        {
            lexer.UngetToken(t);
            int primarytype = parse_primary();
            parse_relop();
            int primarytype2 = parse_primary();
            if(primarytype == -1)
            {
                primarytype = insertnode(getid, false);
            }
            if(primarytype2 == -1)
            {
                primarytype2 = insertnode(getid, false);
            }
            int newtype = asstype(primarytype, primarytype2);
            if(newtype == -1 && eflag==false)
            {
                printerror = "TYPE MISMATCH "+to_string(t.line_no)+" "+"C3";
                eflag = true;
            }    
        }
        else
        {
            int tno = -1;
            for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
            {
                if(i->name == t.lexeme)
                {
                    tno = i-> typeno;
                    break;
                }
            }
            if(tno != BOOLEAN && eflag == false)
            {
                printerror = "TYPE MISMATCH "+to_string(t.line_no)+" "+"C4";
            }
        }
    }
    else
    {
        syntax_error();
    }
}

int Parser::parse_primary()
{
    // primary -> ID
    // primary -> NUM
    // primary -> REALNUM
    // TODO
    Token t = lexer.GetToken();
    if(t.token_type == ID)
    {
        usee ne;
        ne.n2 = t.lexeme;
        ne.useno = t.line_no;
        foruse.push_back(ne);
        getid = t.lexeme;
        int tno = -1;
        for(vector<Symboltable>::iterator i=tnvtable.begin();i!=tnvtable.end();i++)
        {
            if(i->name == t.lexeme)
            {
                tno = i-> typeno;
                break;
            }
        }
        return tno;
    }
    else if(t.token_type == NUM)
    {
        return INTTYPE;
    }
    else if(t.token_type == REALNUM)
    {
        return REALTYPE;
    }
    else
    {
        syntax_error();
    }
}

void Parser::parse_relop()
{
    // relop -> GREATER
    // relop -> GTEQ
    // relop -> LESS
    // relop -> NOTEQ
    // relop -> LTEQ

    // TODO
    Token t = lexer.GetToken();
    if(t.token_type == GREATER)
    {
    
    }
    else if(t.token_type == GTEQ)
    {
    
    }
    else if(t.token_type == LESS)
    {
    
    }
    else if(t.token_type == NOTEQUAL)
    {
    
    }
    else if(t.token_type == LTEQ)
    {
    
    }
    else
    {
        syntax_error();
    }
}

void Parser::ParseInput()
{
    parse_program();
    expect(END_OF_FILE);
    bool a = false,a1=false;
    string tempname = "", tempname1 = "";
    for(vector<decl>::iterator i=fordecl.begin();i!=fordecl.end();i++)
    {
        for(vector<decl>::iterator j=i+1;j!=fordecl.end();j++)
        {
            if(i->n==j->n && eflag==false)
            {
                printerror = "ERROR CODE 1.1 "+i->n;
                eflag=true;
                break;
            }
        }
    }
    for(vector<usee>::iterator i=foruse.begin();i!=foruse.end();i++)
    {
        a=false;
        for(vector<decl>::iterator j=fordecl.begin();j!=fordecl.end();j++)
        {
            if(i->n2==j->n)
            {
                a = true;
            }
        }
        if(a==false)
        {
            printerror = "ERROR CODE 2.4 "+i->n2;
            eflag=true;
            break;
        }
    }
    if(eflag)
    {    cout<<printerror;}
    else
    {
        for(vector<usee>::iterator i=foruse.begin();i!=foruse.end();i++)
        {
            for(vector<decl>::iterator j=fordecl.begin();j!=fordecl.end();j++)
            {
                if(i->n2==j->n)
                    cout<<i->n2<<" "<<i->useno<<" "<<j->declno<<"\n";
            }
        }
    }
}

int main()
{
    Parser parser;
    Symboltable st1;
    st1.name = "INT";
    st1.typeflag = true;
    st1.typeno = tnvcount++;
    st1.lineno = 0;
    st1.insertno = inscount++;
    tnvtable.push_back(st1);
    
    st1.name = "REAL";
    st1.typeflag = true;
    st1.typeno = tnvcount++;
    st1.lineno = 0;
    st1.insertno = inscount++;
    tnvtable.push_back(st1);
    
    st1.name = "BOOLEAN";
    st1.typeflag = true;
    st1.typeno = tnvcount++;
    st1.lineno = 0;
    st1.insertno = inscount++;
    tnvtable.push_back(st1);
    
    st1.name = "STRING";
    st1.typeflag = true;
    st1.typeno = tnvcount++;
    st1.lineno = 0;
    st1.insertno = inscount++;
    tnvtable.push_back(st1);
    
    st1.name = "LONG";
    st1.typeflag = true;
    st1.typeno = tnvcount++;
    st1.lineno = 0;
    st1.insertno = inscount++;
    tnvtable.push_back(st1);
    parser.ParseInput();
}

