#include <cstddef>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "lexer.h"
#include "compiler.h"


using namespace std;

struct id_list
{
    struct ValueNode* val;
    struct id_list *next;
};

#define MAX_TOKEN_LENGTH 200
struct id_list* idList = NULL;
struct id_list* id_head = NULL;
struct id_list* id_tail = NULL;
struct StatementNode* stmt_list_head = NULL;
struct StatementNode* stmt_list_tail = NULL;
struct ValueNode* operand1;
struct ValueNode* operand2;

void add_list(struct id_list*);
void add_stmtList(struct AssignmentStatement*);
void declaration();
struct ValueNode* find_value_node(char*);
struct StatementNode* parse_body();
struct StatementNode* parse_assign_stmt();
struct StatementNode* parse_stmt_list();
struct StatementNode* parse_stmt();
struct StatementNode* parse_assign_stmt();
struct StatementNode* parse_print_stmt();
struct StatementNode* parse_if_stmt();
struct StatementNode* parse_while_stmt();
struct StatementNode* parse_switch_stmt();
struct StatementNode* parse_case_list();
struct StatementNode* parse_for_stmt()

//char token[MAX_TOKEN_LENGTH];      // token string
LexicalAnalyzer lexer;


//int  ttype;                        // token type
ArithmeticOperatorType aot(TokenType tokenType)
{
    if(tokenType == PLUS)
        return OPERATOR_PLUS;
    if(tokenType == MINUS)
        return OPERATOR_MINUS;
    if(tokenType == MULT)
        return OPERATOR_MULT;
    if(tokenType == DIV)
        return OPERATOR_DIV;
    else
        return OPERATOR_NONE;
}

ConditionalOperatorType cot(TokenType tokenType)
{
    if(tokenType == GREATER)
        return CONDITION_GREATER;
    if(tokenType == LESS)
        return CONDITION_LESS;
    if(tokenType == NOTEQUAL)
        return CONDITION_NOTEQUAL;
       
}

struct StatementNode* parse_body()
{
	struct StatementNode* body;
	Token ttype = lexer.GetToken();
	
	if(ttype.token_type != LBRACE)
	{
	     cout << "syntax error!" << endl;
	}
	else
	{
		body = parse_stmt_list();
		return body;
	}
}
struct ValueNode* find_value_node(char* n)
{

	struct id_list* move = id_head;
	while(move != NULL)
	{
		if(strcmp(move->val->name.c_str(), n) == 0)
		{
			return move->val;
		}
		move = move->next;
		
	}

	return NULL;
}

void add_stmtList(struct AssignmentStatement* assign){

	struct StatementNode* stmtNode = new StatementNode();
	stmtNode->type = ASSIGN_STMT;
	stmtNode->assign_stmt = assign;

	if(stmt_list_head == NULL)
	{
		stmt_list_head = stmtNode;
		stmt_list_tail = stmtNode;
	}
	else{
		stmt_list_tail->next = stmtNode;
		stmt_list_tail = stmtNode;
	}
}
void add_list(struct id_list *idl)
{
	if(id_tail == NULL)
	{
		id_head = idl;
		id_tail = idl;
	}
	else
	{
		id_tail->next = idl;
		id_tail = id_tail->next;
	}
}

struct StatementNode* parse_print_stmt(){

	struct PrintStatement* print = new PrintStatement();
	struct StatementNode* stmtNode = new StatementNode();
	stmtNode->type = PRINT_STMT;
	

	Token ttype = lexer.GetToken();
	//ttype = Token LexicalAnalyzer::GetToken();
	if(ttype.token_type == PRINT)
	{
	    ttype= lexer.GetToken();
	    if(ttype.token_type == ID)
	    {
	        print->id = find_value_node(strdup(ttype.lexeme.c_str()));
	        ttype= lexer.GetToken();
            if(ttype.token_type == SEMICOLON)
            {
                
	            stmtNode->type = PRINT_STMT;
	            stmtNode->print_stmt = print;
	            stmtNode->next = NULL;

            }
        }
     
	}
	//ttype = lexer.GetToken();
   

	return stmtNode;
}	
	

struct StatementNode* parse_stmt_list()
{
	
	struct StatementNode* stmt;
	struct StatementNode* stmtList;

	
	stmt = parse_stmt();
	Token ttype =lexer.GetToken();
	if(ttype.token_type == ID || ttype.token_type == PRINT || ttype.token_type == IF || ttype.token_type == WHILE || ttype.token_type == SWITCH || ttype.token_type == FOR)
	{
		lexer.UngetToken(ttype);
		
		stmtList = parse_stmt_list();
		struct StatementNode* move = stmt;
		while(move->next != NULL)
		{
			move = move->next;
		}
		move->next = stmtList;
		
		return stmt;
	}
	else
	{

		lexer.UngetToken(ttype);
		return stmt;
	}
}

struct StatementNode* parse_stmt(){
	
	Token ttype = lexer.GetToken();
	
	if(ttype.token_type == ID)
	{

		lexer.UngetToken(ttype);
		return parse_assign_stmt();

	}
	if(ttype.token_type == PRINT)
	{

		return parse_print_stmt();

	}
	if(ttype.token_type == WHILE)
	{
		
		struct StatementNode* temp = parse_while_stmt();
		temp->next = temp->if_stmt->false_branch;

		struct StatementNode* move = temp->if_stmt->true_branch;
		while(move->next->type != GOTO_STMT){
			move = move->next;
		}
		move->next->goto_stmt->target = temp;

		return temp;
	}
	if(ttype.token_type == IF)
	{

		return parse_if_stmt();

	}
	if(ttype.token_type == SWITCH)
	{

		return parse_switch_stmt();

	}
	if(ttype.token_type == FOR)
	{

		//return parse_for_stmt();

	}
}

struct StatementNode* parse_switch_stmt(){


	Token ttype = lexer.GetToken();
	char* tmpToken = strdup(ttype.lexeme.c_str());
    //char* tmpToken = strdup(ttype.lexeme.c_str());

    ttype = lexer.GetToken();

	struct StatementNode* stmtNode = parse_case_list();

	struct StatementNode* noop = new StatementNode();
	noop->type = NOOP_STMT;

	struct GotoStatement* goTo = new GotoStatement();
	goTo->target = noop;
	struct StatementNode* goStatement = new StatementNode();
	goStatement->type = GOTO_STMT;
	goStatement->goto_stmt = goTo;

	struct StatementNode* move = stmtNode;
	struct StatementNode* move2 = stmtNode;
	while(move->next != NULL)
	{
		if(move->type == IF_STMT)
		{

		    move->if_stmt->condition_operand1 = find_value_node(strdup(tmpToken));
			
			move2 = move->if_stmt->false_branch;
			while(move2->next != NULL){
				move2 = move2->next;
			}
			
			move2->next = goStatement;
		}
		move = move->next;
	} 

	move->next = noop;

	ttype = lexer.GetToken();

	return stmtNode;
}

struct StatementNode* parse_case_list(){

	struct StatementNode* stmtNode = new StatementNode();
	struct IfStatement* ifStmt = new IfStatement();
	
	Token ttype = lexer.GetToken();
	if(ttype.token_type != CASE && ttype.token_type != DEFAULT){
		cout << "CASE or DEFAULT expected" << endl;
		cout << strdup(token) << endl;
		exit(1);
	}

	if(ttype.token_type == CASE){
		
		ttype = lexer.GetToken();
		struct ValueNode* var = new ValueNode();
		var->value = atoi(token);
		
		ifStmt->condition_operand1 = NULL;
		ifStmt->condition_operand2 = var;

		ifStmt->condition_op = CONDITION_NOTEQUAL;

		ttype = lexer.GetToken();

		ttype = lexer.GetToken();

		ifStmt->false_branch = parse_stmt_list();

		struct StatementNode* noop = new StatementNode();
		noop->type = NOOP_STMT;
		
		ifStmt->true_branch = noop;
		
		stmtNode->type = IF_STMT;
		stmtNode->if_stmt = ifStmt;
		stmtNode->next = noop;

		ttype = lexer.GetToken();

		ttype = lexer.GetToken();
		if(ttype.token_type == CASE || ttype.token_type == DEFAULT){
			lexer.UngetToken(ttype);
			stmtNode->next->next = parse_case_list();
		}
		else{
			lexer.UngetToken(ttype);
		}

		return stmtNode;
	}

	else if(ttype.token_type == DEFAULT){
		
		ttype = lexer.GetToken();
		
		ttype = lexer.GetToken();

		stmtNode = parse_stmt_list();

		ttype = lexer.GetToken();

		return stmtNode;

	}
}

struct StatementNode* parse_while_stmt(){

	struct IfStatement* whileStmt = new IfStatement();
	struct StatementNode* sN = new StatementNode();
	Token ttype = lexer.GetToken();
	
	if(ttype.token_type == ID)
	{
		whileStmt->condition_operand1 = find_value_node(strdup(token));
	}
	else
	{
		struct ValueNode* left = new ValueNode();
		left->value = atoi(token);
		whileStmt->condition_operand1 = left;
	}

	ttype = lexer.GetToken();
	
	if(ttype.token_type == GREATER || ttype.token_type == LESS || ttype.token_type == NOTEQUAL)
	{
		whileStmt->condition_op = cot(ttype.token_type);
	}

	ttype = lexer.GetToken();
	
	if(ttype.token_type == ID)
	{
		whileStmt->condition_operand2 = find_value_node(strdup(token));
	}
	else
	{
		struct ValueNode* right = new ValueNode();
		right->value = atoi(token);
		whileStmt->condition_operand2 = right;
	}

	ttype = lexer.GetToken();
	
	whileStmt->true_branch = parse_stmt_list();

	struct GotoStatement* goTo = new GotoStatement();
	struct StatementNode* goToHolder = new StatementNode();
	goToHolder->type = GOTO_STMT;

	goToHolder->goto_stmt = goTo;

	struct StatementNode* iterator = new StatementNode();
	iterator = whileStmt->true_branch;
	while(iterator->next != NULL)
	{
		iterator = iterator->next;
	}
	iterator->next = goToHolder;

	struct StatementNode* noop = new StatementNode();
	noop->type = NOOP_STMT;

	whileStmt->false_branch = noop;

	sN->type = IF_STMT;
	sN->if_stmt = whileStmt;

	ttype = lexer.GetToken();

	return sN;


    
}

struct StatementNode* parse_if_stmt(){

	struct StatementNode* stmtNode = new StatementNode();
	struct IfStatement* ifStmt = new IfStatement();

	
	Token ttype = lexer.GetToken();
	if(ttype.token_type == ID){
		ifStmt->condition_operand1 = find_value_node(strdup(token));
	}
	else{
		struct ValueNode* left = new ValueNode();
		left->value = atoi(token);
		ifStmt->condition_operand1 = left;
	}


	ttype = lexer.GetToken();
	if(ttype.token_type == GREATER || ttype.token_type == LESS || ttype.token_type == NOTEQUAL)
	{
		ifStmt->condition_op = cot(ttype.token_type);
	}

	
	ttype = lexer.GetToken();
	if(ttype.token_type == ID){
		ifStmt->condition_operand2 = find_value_node(strdup(token));
	}
	else{
		struct ValueNode* right = new ValueNode();
		right->value = atoi(token);
		ifStmt->condition_operand2 = right;
	}

	
	ttype = lexer.GetToken();

	ifStmt->true_branch = parse_stmt_list();
	struct StatementNode* move = new StatementNode();
	move = ifStmt->true_branch;
	while(move->next != NULL)
	{
		move = move->next;
	}

	struct StatementNode* noop = new StatementNode();
	noop->type = NOOP_STMT;
	move->next = noop;
	ifStmt->false_branch = noop;

	stmtNode->type = IF_STMT;
	stmtNode->if_stmt = ifStmt;
	stmtNode->next = noop;


	ttype = lexer.GetToken();

	return stmtNode;
}

struct StatementNode* parse_for_stmt(){

    struct StatementNode* stmtNode = new StatementNode();
    Token ttype =lexer.GetToken();
    if(ttype.token_type == FOR)
    {
        ttype = lexer.GetToken();
        if(ttype.token_type == LPAREN)
        {
            parse_assign_stmt();
            parse_condition_stmt();
            ttype = lexer.GetToken();
            if(ttype.token_type == SEMICOLON)
            {
                parse_assign_stmt();
                ttype.lexer.GetToken();
                if(ttype.token_type == RPAREN)
                {
                    parse_body();
                }
                else
                {
                    lexer.UngetToken(ttype);
                }
            }
            lexer.UngetToken(ttype);
            
        }
        lexer.UngetToken(ttype);
    }
    lexer.UngetToken(ttype);

}

struct StatementNode* parse_assign_stmt(){

	//int op = 0;
	ArithmeticOperatorType op;
	Token ttype = lexer.GetToken();
	
	struct ValueNode* t_Id = find_value_node(strdup(ttype.lexeme.c_str()));
	
	if(t_Id == NULL)
	{
		cout << "error: undeclared variable" << endl;
	}
	else
	{
		Token ttype = lexer.GetToken();
		
		if(ttype.token_type == EQUAL)
		{
			Token ttype = lexer.GetToken();
			if(ttype.token_type == ID || ttype.token_type == NUM)
			{
				if(ttype.token_type == ID)
				{
					operand1 = find_value_node(strdup(ttype.lexeme.c_str()));
				}
				else
				{
					struct ValueNode* lhs_val = new ValueNode();
					lhs_val->value = atoi(strdup(ttype.lexeme.c_str());
					operand1 = lhs_val;
				}
				ttype = lexer.GetToken();
				
				if(ttype.token_type == SEMICOLON)
				{
					struct AssignmentStatement* assign = new AssignmentStatement();
					/*assign->left_hand_side = t_Id;
					assign->operand1 = operand1;
					//assign->operand2 = operand2;
					assign->op = op;

					struct StatementNode* stmt = new StatementNode();
					stmt->type = ASSIGN_STMT;
					stmt->assign_stmt = assign;
					stmt->next = NULL;*/
					
					assign->left_hand_side = t_Id;
	                assign->operand1 = operand1;
	                assign->operand2 = NULL;
	                assign->op = NOOP_STMT;

	                struct StatementNode* stmt = new StatementNode();
	                stmt = NULL;
	                stmt->type = ASSIGN_STMT;
	                stmt->assign_stmt = assign;
	                stmt->next = NULL
					return stmt;
					
				}
				else if(ttype.token_type == PLUS || ttype.token_type == MINUS || ttype.token_type == DIV || ttype.token_type == MULT)
				{
					struct AssignmentStatement* assign = new AssignmentStatement();
					assign->op = aot(ttype.token_type);
					ttype = lexer.GetToken();
				
					if(ttype.token_type == ID)
					{
						operand2 = find_value_node(strdup(ttype.lexeme.c_str()));
					}
					else if(ttype.token_type == NUM)
					{
						struct ValueNode* right_hand_side_val = new ValueNode();
						right_hand_side_val->value = atoi(strdup(ttype.lexeme.c_str()));
						operand2 = right_hand_side_val;
					}
				}
			}
		}
	}

	struct AssignmentStatement* assign = new AssignmentStatement();
	
	assign->left_hand_side = t_Id;
	assign->operand1 = operand1;
	assign->operand2 = operand2;
	assign->op = op;

	struct StatementNode* stmt = new StatementNode();
	//stmt = NULL;
	stmt->type = ASSIGN_STMT;
	stmt->assign_stmt = assign;
	stmt->next = NULL;

	ttype = lexer.GetToken();

	return stmt;
	
}


void declaration()
{

	Token ttype = lexer.GetToken();
	
	if(ttype.token_type == ID){
		
		struct ValueNode *val = new ValueNode();
		val->name = strdup(ttype.lexeme.c_str());
		val->value = 0;

		struct ValueNode *val2 = new ValueNode();
		val2->value = 0;

		struct AssignmentStatement *assign = new AssignmentStatement();
		assign->left_hand_side = val;
		assign->operand1 = val2;
		assign->operand2 = NULL;
		//assign->op = 0;

		struct id_list *id = new id_list();
		id->val = val;
		id->next = NULL;
		add_list(id);
		
		add_stmtList(assign);

		Token ttype = lexer.GetToken();

		if(ttype.token_type == COMMA)
		{
			declaration();
		}
		else if (ttype.token_type == SEMICOLON)
		{
			return;
		}
		else
		{
		    cout << "syntax error!" << endl;
		 
		}
	}
}

struct StatementNode* parse_generate_intermediate_representation()
{
	
	declaration();
	stmt_list_tail->next = parse_body();
	return stmt_list_head;
}

