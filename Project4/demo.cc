#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "lexer.h"
#include<map>
using namespace std;
struct StatementNode* parse_generate_intermediate_representation();
LexicalAnalyzer lexer;
struct StatementNode* parse_program();
void parse_var_section();
void parse_id_list();
struct StatementNode* parse_body();
struct StatementNode* parse_stmt_list();
struct StatementNode* parse_stmt();
struct StatementNode* parse_assign_stmt();
struct StatementNode* parse_expr();
struct StatementNode* parse_print_stmt();
struct StatementNode* parse_while_stmt();
struct StatementNode* parse_if_stmt();
struct StatementNode* parse_switch_stmt();
struct StatementNode* parse_for_stmt();
multimap<string,struct ValueNode*> stable;
struct StatementNode* parse_generate_intermediate_representation()
{
    return parse_program();
}
Token peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

Token expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    return t;
}

struct StatementNode* parse_program()
{
    parse_var_section();
    struct StatementNode * sn = parse_body();
    return sn;
}

void parse_var_section()
{
    Token t = peek();
    if(t.token_type == ID)
    {
        parse_id_list();
    }  
     
}
void parse_id_list()
{
    Token t = lexer.GetToken();
    if(t.token_type == ID)
    {
        ValueNode *vn = new ValueNode;
        vn->name = t.lexeme;
        vn->value = 0;
        stable.insert(pair<string, struct ValueNode*>(t.lexeme, vn));
        Token t1=lexer.GetToken();
        if(t1.token_type == COMMA)
        {
            parse_id_list();
        }    
    }
}
struct StatementNode* parse_body()
{
    expect(LBRACE);
    struct StatementNode *sn = parse_stmt_list();
    expect(RBRACE);
    return sn;
}
struct StatementNode* parse_stmt_list()
{
    struct StatementNode *sn = parse_stmt();
    Token t = peek();
    if(t.token_type == ID || t.token_type == PRINT || t.token_type == WHILE || t.token_type == IF || t.token_type == SWITCH || t.token_type == FOR)
    {
        struct StatementNode *temp = sn;
        while(temp->next!=NULL)
        {
            temp = temp->next;
        }
        temp->next = parse_stmt_list();
    }
    else if(t.token_type == RBRACE)
    {
    }
    return sn;
}
struct StatementNode* parse_stmt()
{
    Token t = peek();
    if(t.token_type == ID)
    {
        return parse_assign_stmt();
    }
    else if(t.token_type == PRINT)
    {
        return parse_print_stmt();
    }
    else if(t.token_type == WHILE)
    {
        return parse_while_stmt();
    }
    else if(t.token_type == IF)
    {
        return parse_if_stmt();
    }
    else if(t.token_type == SWITCH)
    {
        return parse_switch_stmt();
    }
    else if(t.token_type == FOR)
    {
        return parse_for_stmt();
    }
}
struct StatementNode* parse_assign_stmt()
{
    struct StatementNode *sn = new StatementNode;
    struct AssignmentStatement *an = new AssignmentStatement;
    Token t= lexer.GetToken();
    if(t.token_type == ID)
    {
        an->left_hand_side = stable.find(t.lexeme)->second;
        Token t2= lexer.GetToken();
        if(t2.token_type == EQUAL )
        {
            Token t3= lexer.GetToken();
            if(t3.token_type == ID || t3.token_type == NUM)
            {
                ValueNode *op1 = new ValueNode;
                if(t3.token_type==ID)
                {
                    op1->name= t3.lexeme;
                    op1->value = 0;
                    an->operand1=stable.find(t3.lexeme)->second;
                }    
                else if(t3.token_type==NUM)
                {
                    op1->value = atoi(t3.lexeme.c_str());
                    an->operand1=op1;     
                } 
                Token t4= lexer.GetToken();
                if(t4.token_type == SEMICOLON)
                {         
                    an->operand2 = NULL;
                    an->op = OPERATOR_NONE;
                    sn->type = ASSIGN_STMT;
                    sn->assign_stmt = an;
                    sn->next=NULL;
                    return sn;
                }
                else
                { 
                    if(t4.token_type == PLUS)
                    {
                        an->op = OPERATOR_PLUS;
                    }
                    else if(t4.token_type == MINUS)
                    {
                        an->op = OPERATOR_MINUS;
                    }
                    else if(t4.token_type == MULT)
                    {
                        an->op = OPERATOR_MULT;
                    }
                    else if(t4.token_type == DIV)
                    {
                        an->op = OPERATOR_DIV;
                    }
                    Token t5 = lexer.GetToken();
                    ValueNode *op2 = new ValueNode;
                    if(t5.token_type == ID)
                    {
                        op2->name = t5.lexeme;
                        op2->value = 0;
                        an->operand2=stable.find(t5.lexeme)->second;
                    }
                    else if(t5.token_type == NUM)
                    {                
                        op2->value = atoi(t5.lexeme.c_str());      
                        an->operand2=op2;                   
                    }
                    Token t6= lexer.GetToken();
                    if(t6.token_type == SEMICOLON)
                    {
                        sn->type = ASSIGN_STMT;
                        sn->assign_stmt = an;
                        sn->next=NULL;                           
                        return sn;
                    }         
                }        
            }
        }
    }    
}
struct StatementNode* parse_print_stmt()
{
    Token t = lexer.GetToken();
    if(t.token_type == PRINT)
    {
        Token t1 = lexer.GetToken();
        if(t1.token_type == ID)
        {
            Token t2 = lexer.GetToken();
            if(t2.token_type == SEMICOLON)
            {
                PrintStatement *ps = new PrintStatement;
                ps->id = stable.find(t1.lexeme)->second;
                StatementNode *sn = new StatementNode;
                sn->type = PRINT_STMT;
                sn->print_stmt = ps;
                sn->next = NULL;
                return sn;  
            }
        }
    }
}

struct StatementNode* parse_if_stmt()
{
    StatementNode *sn = new StatementNode;
    IfStatement *is = new IfStatement;
    Token t = lexer.GetToken();
    Token t1 = lexer.GetToken();
    if(t1.token_type == NUM)
    {
        ValueNode *op1 = new ValueNode;
        op1->value = atoi(t1.lexeme.c_str());
        is->condition_operand1 = op1;
    }
    else if(t1.token_type == ID)
    {
        is->condition_operand1 = stable.find(t1.lexeme)->second;
    }
    Token t2 = lexer.GetToken();
    if(t2.token_type == GREATER)
    {
        is->condition_op = CONDITION_GREATER;
    }
    else if(t2.token_type == LESS)
    {
        is->condition_op = CONDITION_LESS;
    }
    else if(t2.token_type == NOTEQUAL)
    {
        is->condition_op = CONDITION_NOTEQUAL;
    }
    Token t3 = lexer.GetToken();
    if(t3.token_type == NUM)
    {
        ValueNode *op2 = new ValueNode;
        op2->value = atoi(t1.lexeme.c_str());
        is->condition_operand2 = op2;
    }
    else if(t3.token_type == ID)
    {
        is->condition_operand2 = stable.find(t3.lexeme)->second;
    }
    StatementNode *sn1 = new StatementNode;
    sn1->type = NOOP_STMT;
    sn1->next = NULL;
    is->true_branch = parse_body();
    StatementNode *temp = is->true_branch;
    while(temp->next!=NULL)
    {
        temp = temp->next;
    }
    temp->next = sn1;
    is->false_branch = sn1;
    sn->type = IF_STMT;
    sn->if_stmt = is;
    sn->next = sn1;
    return sn;
}

struct StatementNode* parse_while_stmt()
{
    StatementNode *sn = new StatementNode;
    IfStatement *is = new IfStatement;
    sn->type = IF_STMT;
    sn->if_stmt = is;
    expect(WHILE);
    Token t1 = lexer.GetToken();
    if(t1.token_type == NUM)
    {
        ValueNode *op1 = new ValueNode;
        op1->value = atoi(t1.lexeme.c_str());
        is->condition_operand1 = op1;
    }
    else if(t1.token_type == ID)
    {
        is->condition_operand1 = stable.find(t1.lexeme)->second;
    }
    Token t2 = lexer.GetToken();
    if(t2.token_type == GREATER)
    {
        is->condition_op = CONDITION_GREATER;
    }
    else if(t2.token_type == LESS)
    {
        is->condition_op = CONDITION_LESS;
    }
    else if(t2.token_type == NOTEQUAL)
    {
        is->condition_op = CONDITION_NOTEQUAL;
    }
    Token t3 = lexer.GetToken();
    if(t3.token_type == NUM)
    {
        ValueNode *op2 = new ValueNode;
        op2->value = atoi(t1.lexeme.c_str());
        is->condition_operand2 = op2;
    }
    else if(t3.token_type == ID)
    {
        is->condition_operand2 = stable.find(t3.lexeme)->second;
    }
    struct GotoStatement *gs = new GotoStatement;
    struct StatementNode *sn1 = new StatementNode;
    sn1->type = GOTO_STMT;
    sn1->goto_stmt = gs;
    gs->target = sn;
    is->true_branch = parse_body();
    struct StatementNode *temp = is->true_branch;
    while(temp->next!=NULL)
    {
        temp = temp->next;
    }
    temp->next = sn1;
    struct StatementNode *ns = new StatementNode;
    ns->type = NOOP_STMT;
    ns->next = NULL;
    is->false_branch = ns;
    sn->next = ns;
    return sn;
}

struct StatementNode* parse_switch_stmt()
{
    expect(SWITCH);
    Token t = lexer.GetToken();
    expect(LBRACE); 
    struct IfStatement *is = new IfStatement;
    struct StatementNode *sn = new StatementNode;
    struct StatementNode *s1 = NULL;
    struct StatementNode *s2 = NULL;
    Token t1= lexer.GetToken();
    while(t1.token_type != RBRACE)
    {
        if(t1.token_type == CASE)
        {
            ValueNode *op1 = stable.find(t.lexeme)->second;
            is->condition_operand1 = op1;
            is->condition_op = CONDITION_NOTEQUAL;
            Token t2 = lexer.GetToken();
            ValueNode *op2 = new ValueNode;
            op2->value = atoi(t2.lexeme.c_str());
            is->condition_operand2= op2;
            expect(COLON);
            is->false_branch = parse_body();
            StatementNode *nss = new StatementNode;
            GotoStatement *gs = new GotoStatement;
            nss->type=GOTO_STMT;
            nss->goto_stmt=gs;
            gs->target=NULL;
            struct StatementNode *sn1 = new StatementNode;
            sn1->type=NOOP_STMT;
            sn1->next= NULL;
            struct StatementNode *temp =is->false_branch;
            while(temp->next!=NULL)
            {
                temp=temp->next;
            } 
            temp->next=nss; 
            is->true_branch=sn1;
            sn->type=IF_STMT;
            sn->if_stmt = is;
            sn->next=sn1;
            if(s1 != NULL)
            {
                s1->next->next=sn;
                s1=s1->next->next;
            }
            else
            {
                s1=sn;
                s2=sn;
            }
        }
        else if(t1.token_type == DEFAULT)    
        {
            expect(COLON);
            struct StatementNode *sn3 = parse_body();
            struct StatementNode *sn2 = new StatementNode;
            sn2->type=NOOP_STMT;
            sn2->next= NULL;
            sn3->next=sn2;
            if(s1!=NULL)
            {
                s1->next->next=sn3;
                s1=s1->next->next;  
            }
            else
            {
                s1=sn3;
                s2=s1;
            }
        }
        t1= lexer.GetToken();
    }
    struct StatementNode *temp = s2;
    while(temp->next!=NULL)
    {
        if(temp->type == IF_STMT)
        {
            struct StatementNode *temp1 = temp->if_stmt->false_branch;
            while(temp1->next!=NULL)
            {
                temp1=temp1->next;
            }
            temp1->goto_stmt->target=s1->next; 
        }
        temp=temp->next;
    } 
    return s2;
}
struct StatementNode* parse_for_stmt()
{
    struct StatementNode *sn = new StatementNode;
    sn->next=NULL;
    sn->type=IF_STMT;
    struct IfStatement *is = new IfStatement;
    sn->if_stmt=is;
    expect(FOR);
    expect(LPAREN);
    struct StatementNode *as = parse_assign_stmt();
    as->next=sn;
    Token t = lexer.GetToken();
    if(t.token_type == NUM)
    {
        ValueNode *op1 = new ValueNode;
        op1->value = atoi(t.lexeme.c_str());
        is->condition_operand1= op1;
    }
    else if(t.token_type == ID)
    {
        is->condition_operand1= stable.find(t.lexeme)->second;
        
    }
    Token t2 = lexer.GetToken();
    if(t2.token_type == GREATER)
    {
        is->condition_op = CONDITION_GREATER;
    }
    else if(t2.token_type == LESS)
    {
        is->condition_op = CONDITION_LESS;
    }
    else if(t2.token_type == NOTEQUAL)
    {
        is->condition_op = CONDITION_NOTEQUAL;
    }
    Token t3 = lexer.GetToken();
    if(t3.token_type == NUM)
    {
        ValueNode *op2 = new ValueNode;
        op2->value = atoi(t3.lexeme.c_str());
        is->condition_operand2= op2;
    } 
    else if(t3.token_type == ID)
    {
        is->condition_operand2= stable.find(t3.lexeme)->second;   
    }
    expect(SEMICOLON);
    struct StatementNode *as2 = parse_assign_stmt();
    expect(RPAREN);
    is->true_branch = parse_body();
    GotoStatement *gs = new GotoStatement;
    StatementNode *sn1 = new StatementNode;
    sn1->type=GOTO_STMT; 
    gs->target=sn;
    sn1->goto_stmt=gs;
    struct StatementNode *temp =is->true_branch;
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=as2;
    as2->next=sn1;
    struct StatementNode *ns = new StatementNode;
    ns->type=NOOP_STMT;
    ns->next= NULL;
    is->false_branch=ns;
    sn->next=ns;
    return as;
}

/*struct StatementNode * parse_generate_intermediate_representation()
{
    // Sample program for demonstration purpose only
    // Replace the following with a parser that reads the program from stdin &
    // creates appropriate data structures to be executed by execute_program()
    // This is the imaginary input for the following construction:
    // a, b;
    // {
    //    a = 10;
    //    b = 1;
    //    WHILE a > 0
    //    {
    //        b = b * a;
    //        a = a - 1;
    //    }
    //    print b;
    // }

    struct ValueNode * A = new ValueNode;
    struct ValueNode * B = new ValueNode;
    struct ValueNode * ONE = new ValueNode;
    struct ValueNode * TEN = new ValueNode;
    struct ValueNode * ZERO = new ValueNode;
    struct StatementNode * s1 = new StatementNode;
    struct StatementNode * s2 = new StatementNode;
    struct StatementNode * s3 = new StatementNode;
    struct StatementNode * s4 = new StatementNode;
    struct StatementNode * s5 = new StatementNode;
    struct StatementNode * s6 = new StatementNode;
    struct StatementNode * s7 = new StatementNode;
    struct StatementNode * s8 = new StatementNode;

    A->name = "a";      A->value = 0;
    B->name = "b";      B->value = 0;
    ONE->name = "";     ONE->value = 1;
    TEN->name = "";     TEN->value = 10;
    ZERO->name = "";    ZERO->value = 0;

    s1->type = ASSIGN_STMT;
    s1->assign_stmt = new AssignmentStatement;
    s1->assign_stmt->left_hand_side = A;
    s1->assign_stmt->op = OPERATOR_NONE;
    s1->assign_stmt->operand1 = TEN;
    s1->assign_stmt->operand2 = NULL;
    s1->next = s2;

    s2->type = ASSIGN_STMT;
    s2->assign_stmt = new AssignmentStatement;
    s2->assign_stmt->left_hand_side = B;
    s2->assign_stmt->op = OPERATOR_NONE;
    s2->assign_stmt->operand1 = ONE;
    s2->assign_stmt->operand2 = NULL;
    s2->next = s3;

    s3->type = IF_STMT;
    s3->if_stmt = new IfStatement;
    s3->if_stmt->condition_op = CONDITION_GREATER;
    s3->if_stmt->condition_operand1 = A;
    s3->if_stmt->condition_operand2 = ZERO;
    s3->if_stmt->true_branch = s4;
    s3->if_stmt->false_branch = s7;
    s3->next = s7;

    s4->type = ASSIGN_STMT;
    s4->assign_stmt = new AssignmentStatement;
    s4->assign_stmt->left_hand_side = B;
    s4->assign_stmt->op = OPERATOR_MULT;
    s4->assign_stmt->operand1 = B;
    s4->assign_stmt->operand2 = A;
    s4->next = s5;

    s5->type = ASSIGN_STMT;
    s5->assign_stmt = new AssignmentStatement;
    s5->assign_stmt->left_hand_side = A;
    s5->assign_stmt->op = OPERATOR_MINUS;
    s5->assign_stmt->operand1 = A;
    s5->assign_stmt->operand2 = ONE;
    s5->next = s6;

    s6->type = GOTO_STMT;
    s6->goto_stmt = new GotoStatement;
    s6->goto_stmt->target = s3;    // Jump to the if statement
    s6->next = s7;

    s7->type = NOOP_STMT;
    s7->next = s8;

    s8->type = PRINT_STMT;
    s8->print_stmt = new PrintStatement;
    s8->print_stmt->id = B;
    s8->next = NULL;

    return s1;
}*/


