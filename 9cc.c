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
typedef struct{
	int ty;		//token type
	int val;	//if ty is TK_NUM, that value
	char *input;	//token strings(use for error message
} Token;

//store tokenized result tokenized string in this array
//No more than 100 tokens shall come
Token tokens[100];

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

//function to report an error
void error(int i)
{
	fprintf(stderr, "Unexpected token: %s\n", tokens[i].input);
	exit(1);
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

	//output front part of assembly
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	//check for first token, the first token of the expression must be numeric
	//and output first mov instruction
	if(tokens[0].ty != TK_NUM)
		error(0);	
	printf("	mov rax, %d\n", tokens[0].val);
	
	//'+ <numver>' or '- <number>' tokens consumpition
	//ad output assembly
	int i = 1;
	while(tokens[i].ty != TK_EOF)
	{
		if(tokens[i].ty == '+')
		{
			i++;
			if(tokens[i].ty != TK_NUM)
				error(i);
			printf("	add rax, %d\n", tokens[i].val);
			i++;
			continue;
		}
		
		if(tokens[i].ty = '-')
		{
			i++;
			if(tokens[i].ty != TK_NUM)
				error(i);
			printf("	sub rax, %d\n", tokens[i].val);
			i++;
			continue;
		}

		error(i);
	}

	printf("	ret\n");
	return 0;
}
