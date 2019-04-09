#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//value of token type
enum
{
	TK_NUM = 256,	//integer token
	TK_EOF,		//express end of input(file) token
};

//token type
typedef struct
{
	int ty;		//token type
	int val;	//if ty is TK_NUM, that value
	char *input;	//token strings(use for error message
} Token;

//value of node type
enum
{
	ND_NUM = 256,	//integer node type
};

//Node type
typedef struct Node
{
	int ty;			//operater or ND_NUM
	struct Node *lhs;	//left-hand side
	struct Node *rhs;	//right-hand side
	int val;		//use if ty is ND_NUM
} Node;

//store tokenized result tokenized string in this array
//No more than 100 tokens shall come
Token tokens[100];
int pos = 0; 	//tokens index number

//divide the string pointed to by p into tokens and store them in tokens
void tokenize(char *p)
{
	int i = 0;
	while(*p)
	{	
		//skip space character
		if(isspace(*p))
		{
			p++;
			continue;
		}

		if(*p == '+' || *p == '-')
		{
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}

		if(isdigit(*p))
		{
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p, &p, 10);
			i++;
			continue;
		}

		fprintf(stderr, "Cannot tokenize: %s\n", p);
		exit(1);
	}
	
	tokens[i].ty = TK_EOF;
	tokens[i].input = p;
}

Node *new_node(int ty, Node *lhs, Node *rhs)
{
	Node *node = malloc(sizeof(Node));
	node->ty = ty;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node *new_node_num(int val)
{
	Node *node = malloc(sizeof(Node));
	node->ty = ND_NUM;
	node->val = val;
	return node;
}

Node *term()
{
	//if(consume('('))
	//{
	//	Node *node = add();
	//	if(!consume(')'))
	//		error("nothing parenthesis pair: %s", tokens[pos].input);
	//	return node;
	//}

	if(tokens[pos].ty == TK_NUM)
		return new_node_num(tokens[pos++].val);

	error("tokens is nor number or open parenthesis: %s",tokens[pos].input);
}

Node *add()
{
	Node *node = term();
	
	if(consume('+'))
		node = new_node('+', node, term());
	if(consume('-'))
		node = new_node('-', node, term());
	return node;
}

int consume(int ty)
{
	if(tokens[pos].ty != ty)
		return 0;
	pos++;
	return 1;
}

//function to report an error
void error(int i)
{
	fprintf(stderr, "Unexpected token: %s\n", tokens[i].input);
	exit(1);
}	

void gen(Node *node)
{
	//if node is number, node value push in the register
	if(node->ty == ND_NUM)
	{
		printf("	push %d\n", node->val);
		return;
	}
	
	//Recursion gen,
	gen(node->lhs);
	gen(node->rhs);

	//operate, pop and calc
	printf("	pop rdi\n");
	printf("	pop rax\n");

	switch(node->ty)
	{
		case '+':
			printf("	add rax, rdi\n");
			break;
		case '-':
			printf("	sub rax, rdi\n");
			break;
		case '*':
			printf("	mul rdi\n");
			break;
		case '/':
			printf("	mov rdx, 0\n");
			printf("	div rdi\n");
	}

	printf("	push rax\n");
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Number of incorrect arguments");
		return 1;
	}
	
	//tokenize...
	tokenize(argv[1]);
	Node *node = add();

	//output front part of assembly
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	//check for first token, the first token of the expression must be numeric
	//and output first mov instruction
	if(tokens[0].ty != TK_NUM)
		error(0);	

	gen(node);

	printf("	pop rax\n");
	printf("	ret\n");
	return 0;
}
