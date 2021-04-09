#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "LL.h"

typedef struct node {
    char input[10];
    short type;
} node;


node words[1000];
int len = 0;
int parseindex = 0;

int lexer(char* file);
void parser(void);
STACK pop (STACK stack);
void push (STACK stack, int Type, int val);
int Action(STACK stack);
int Dojob(STACK stack, int nonT, int input);

	
int main(int argc, char** argv)
{
	if (argv[1] == NULL){
		return 0;
	}
	else{
		lexer(argv[1]);
	}

	for(int i = 0; i<len; i++){
		if( words[i].type == FUNC )
			words[i].type = WORD;
	}

	parser();
}


int lexer(char* filename)
{
    FILE *in = fopen(filename,"r");

    for(int i=0;;i++)
    {
        char b[30];
        if(EOF==fscanf(in,"%s",b)) break;
        if(!strcmp(b,"BEGIN"))
        {
            fscanf(in,"%s",b);
            break;
        }
    }
    for(int i=0;;i++) 
    {
        char b[30];
        if(EOF==fscanf(in,"%s",b)) break;
        if(!strcmp(b,"END")) break;
        int cnnt=0;
        while(1)
        {
            int ck=0;
            for(int j=1;j<strlen(b);j++)
            {
                if(b[j]=='='&&b[j+1]=='=')
                {
                    cnnt++;
                    ck=1;
                    char c1[10],c2[2],c3[10];
                    int cnt=0;
                    for(int k=0;k<j;k++)
                        c1[cnt++]=b[k];
                    cnt=0;
                    c2[0]=b[j];
                    c2[1]=b[j+1];
                    int ck2=0;
                    for(int k=j+2;k<strlen(b);k++,ck2++)
                        c3[cnt++]=b[k];
                    if(ck2!=0) 
                    {
                        len+=2;
                        strcpy(words[i++].input,c1);
                        strcpy(words[i++].input,c2);
                        strcpy(b,c3);
                    }
                    else 
                    {
                        len+=2;
                        strcpy(words[i++].input,c1);
                        strcpy(words[i++].input,c2);
                        ck=0;
                    }
                    break;
                }
                if(b[j]=='{' || b[j]=='}' || b[j] =='(' || b[j]==')' || b[j]==';' || b[j]=='>' || b[j]=='=' || b[j]=='+' || b[j]=='*' || b[j]==',')
                {
                    cnnt++;
                    ck=1;
                    char c1[10],c2,c3[10];
                    int cnt=0;
                    for(int k=0;k<j;k++)
                        c1[cnt++]=b[k];
                    c1[cnt]=0;
                    cnt=0;
                    c2=b[j];
                    int ck2=0;
                    for(int k=j+1;k<strlen(b);k++,ck2++)
                        c3[cnt++]=b[k];
                    c3[cnt]=0;
                    if(ck2!=0) 
                    {
                        len+=2;
                        strcpy(words[i++].input,c1);
                        words[i++].input[0]=c2;
                        strcpy(b,c3);
                    }
                    else 
                    {
                        len+=2;
                        strcpy(words[i++].input,c1);
                        words[i++].input[0]=c2;
                        ck=0;
                    }
                    break;
                }
            }
            if(ck==0) break;
        }
        if(cnnt>0) {
            i--;
            continue;
        }
        len++;
        strcpy(words[i].input,b);
    
    }

    for(int i=0;i<len;i++) 
    {
        if(!strcmp(words[i].input,"int")) words[i].type=INT;
        else if(!strcmp(words[i].input,"char")) words[i].type=CHAR;
        else if(!strcmp(words[i].input,"RETURN")) words[i].type=RETURN;
        else if(!strcmp(words[i].input,"WHILE")) words[i].type=WHILE;
        else if(!strcmp(words[i].input,"IF")) words[i].type=IF;
        else if(!strcmp(words[i].input,"THEN")) words[i].type=THEN;
        else if(!strcmp(words[i].input,"ELSE")) words[i].type=ELSE;
        else if(!strcmp(words[i].input,"{")) words[i].type=L_Brace;
        else if(!strcmp(words[i].input,"}")) words[i].type=R_Brace;
        else if(!strcmp(words[i].input,"(")) words[i].type=L_Paren;
        else if(!strcmp(words[i].input,")")) words[i].type=R_Paren;
        else if(!strcmp(words[i].input,"=")) words[i].type=equal;
        else if(!strcmp(words[i].input,";")) words[i].type=colon;
        else if(!strcmp(words[i].input,">")) words[i].type=quality;
        else if(!strcmp(words[i].input,"==")) words[i].type=same;
        else if(!strcmp(words[i].input,"+")) words[i].type=add;
        else if(!strcmp(words[i].input,"*")) words[i].type=mul;
        else if(!strcmp(words[i].input,",")) words[i].type=comma;
        else {
            int ck=0;
            for(int j=0;j<strlen(words[i].input);j++)
            {
                if(words[i].input[j]<='Z'&&words[i].input[j]>='A') continue;
                else if(words[i].input[j]<='z'&&words[i].input[j]>='a') continue;
                else ck=1;
            }
            if(ck==0)
            {
                words[i].type=WORD;
                continue;
            }
            ck=0;
            for(int j=0;j<strlen(words[i].input);j++)
            {
                if(words[i].input[j]<='9'&&words[i].input[j]>='0') continue;
                else ck=1;
            }
            if(ck==1)
            {
                printf("input error!\n");
                return 0;
            }
            words[i].type=NUM;
        }
    }
    for(int i=0;i<len-1;i++)
    {
        if(words[i].type==WORD && words[i+1].type==L_Paren) words[i].type=FUNC;
    }
    printf("================INPUT FORMAT==================");
    for(int i=0;i<len;i++)
    {   
		if(i%5 == 0)
		printf("\n");
        printf("%d(%s) \t",words[i].type, words[i].input);
	}
	printf("\n==================INPUT END===================\n");

	fclose(in);
	in = fopen("symbol.txt","w");
	
	fprintf(in,"==================SYMBOLS=================\n");	
	fprintf(in,"FUNCTIONS\n");
	for(int i = 0; i<len; i++){
		if(words[i].type == FUNC)
			fprintf(in,"    %s()\n",words[i].input);
	}	
	fprintf(in,"VARIABLES\n");
	for(int i = 0; i<len; i++){
		if(words[i].type == INT){
            for(int j=i+1;j<len;j++)
            {
                if(words[j].type == WORD) fprintf(in,"    INT \t%s\n",words[j].input);
                else if(words[j].type==colon) break;
            }
        }
		if(words[i].type == CHAR){
            for(int j=i+1;j<len;j++)
            {
                if(words[j].type==WORD)	fprintf(in,"    CHAR \t%s\n",words[j].input);
                else if(words[j].type==colon) break;
            }
		}
	}
	fprintf(in,"==================SYMBOL END=================\n");	
    
	return 0;
}


void parser(void){
	
	STACK stack = (Element*)calloc(1,sizeof(Element));
	stack->type = 0;		//Header

	STACK init = (Element*)calloc(1,sizeof(Element));
	stack->next = init;
	init->type = NON_TERM;
	init->nonterm = Prog;
	init->next = NULL;

	while(parseindex < len){
		if(Action(stack) == 1){
			printf("WRONG GRAMMER.\n");
			return;
		}
	}
	printf("RIGHT GRAMMER.\n");
}

int Action(STACK stack){
	
	int rej = 0;

	if(stack->next == NULL)
	{
		printf("case EMPTY\n");
		return 1;
	}

	if(stack->next->type == TERMINAL){		//TOP IS TERMINAL
		if(stack->next->terminal->type == words[parseindex].type){
			parseindex++;
			pop(stack);
//			printf("case REDUCTION\n");
			return 0;
		}
		else{
			printf("case UNSAME TERMINAL\n");
			return 1;
		}
	}

	if(stack->next->type == NON_TERM){
		rej = Dojob(stack, stack->next->nonterm, words[parseindex].type);
		return rej;
	}

	printf("SOMETHING WRONG NO 12\n");
	return 1;
}

STACK pop (STACK stack){		//RETURN new top

	//printf("POP \n");

	if(stack->next == NULL)
		printf("ERROR : POP on EMPTY\n");
	
	STACK newtop = stack->next->next;
	STACK nowtop = stack->next;

	free(nowtop);
	stack->next = newtop;

	return newtop;
}

void push (STACK stack, int Type, int val){

	//printf("PUSH \n");
	STACK newtop = (STACK)calloc(1,sizeof(Element));
	newtop->type = Type;
	if(Type == NON_TERM){
		newtop->nonterm = val;
	}
	else if (Type == TERMINAL){
		newtop->terminal = (node*)calloc(1,sizeof(node));
		newtop->terminal->type = val;
	}
	
	STACK nowtop = stack->next;
	stack->next = newtop;
	newtop->next = nowtop;
	
	return;
}

int Dojob(STACK stack, int nonT, int input){
	
//	printf("DOJOB : %d, %d\n",nonT, input);

	switch(nonT){
		case Prog:
		{	

			switch(input){
				case FUNC:
				case WORD:
					pop(stack);
					push(stack,NON_TERM,Block);
					push(stack,TERMINAL,R_Paren);
					push(stack,TERMINAL,L_Paren);
					push(stack,TERMINAL,WORD);
					break;
				default :
				return 1;
			}	
		}
		break;

		case Decls:
		{
			switch(input){
				case INT:
				case CHAR:
					pop(stack);
					push(stack,NON_TERM,Decls_);
					push(stack,NON_TERM,Decl);
				break;
				case WORD:
				case IF:
				case WHILE:
				case RETURN:
					pop(stack);
				break;
				default :
				printf("Decls\n");
				return 1;
			}
		}
		break;

		case Decls_:
		{
			switch(input){
				case INT:
				case CHAR:
					pop(stack);
					push(stack,NON_TERM,Decls_);
					push(stack,NON_TERM,Decl);
					break;
				case WORD:
				case IF:
				case WHILE:
				case RETURN:
					pop(stack);
					break;
				default :
				printf("Decls_\n");
					return 1;
			}
		}
		break;

		case Decl:
		{
			switch(input){
				case INT:
				case CHAR:
					pop(stack);
					push(stack, TERMINAL, colon);
					push(stack, NON_TERM, Words);
					push(stack, NON_TERM, Vtype);
					break;
				default :
				printf("Decl\n");
					return 1;
			}
		}
		break;

		case Words:
		{
			switch(input){
				case WORD:
					pop(stack);
					push(stack, NON_TERM, Words_);
					push(stack, TERMINAL, WORD);
					break;
				default :
				printf("Words\n");
					return 1;
			}
		}
		break;

		case Words_:
		{
			switch(input){
				case colon:
					pop(stack);
					break;
				case comma:
					pop(stack);
					push(stack, NON_TERM,Words_);
					push(stack, TERMINAL,WORD);
					push(stack, TERMINAL,comma);
					break;
				default :
				printf("Words_ %d\n",input);
					return 1;
			}
		}
		break;

		case Block:
		{
			switch(input){
				case L_Brace:
					pop(stack);
					push(stack, TERMINAL, R_Brace);
					push(stack, NON_TERM, Slist);
					push(stack, NON_TERM, Decls);
					push(stack, TERMINAL, L_Brace);
					break;
				default :
				printf("Block\n");
					return 1;
			}
		}
		break;

		case Slist:
		{
			switch(input){
				case WORD:
				case IF:
				case WHILE:
				case RETURN:
					pop(stack);
					push(stack, NON_TERM, Slist_);
					push(stack, NON_TERM, Stat);
				break;
			default :
				printf("Slist\n");
				return 1;
			}
		}
		break;

		case Slist_:
		{
			switch(input){
				case WORD:
				case IF:
				case WHILE:
				case RETURN:
					pop(stack);
					push(stack, NON_TERM, Slist_);
					push(stack, NON_TERM, Stat);
				break;
				case R_Brace:
					pop(stack);
				break;
			default :
				printf("Slist_\n");
				return 1;
			}
		}
		break;


		case Stat:
		{
			switch(input){
				case WORD:
					pop(stack);
					push(stack, TERMINAL, colon);
					push(stack, NON_TERM, Expr);
					push(stack, TERMINAL, equal);
					push(stack, TERMINAL, WORD);
					break;

				case IF:
					pop(stack);
					push(stack, NON_TERM, Block);
					push(stack, TERMINAL, ELSE);
					push(stack, NON_TERM, Block);
					push(stack, TERMINAL, THEN);
					push(stack, NON_TERM, Cond);
					push(stack, TERMINAL, IF);
					break;

				case WHILE:
					pop(stack);
					push(stack, NON_TERM, Block);
					push(stack, NON_TERM, Cond);
					push(stack, TERMINAL, WHILE);
					break;	

				case RETURN:
					pop(stack);
					push(stack, TERMINAL, colon);
					push(stack, NON_TERM, Expr);
					push(stack, TERMINAL, RETURN);
					break;
				default :
				printf("Stat\n");
					return 1;
			}
		}
		break;
		
		case Cond:
		{
			switch(input){
				case WORD:
				case NUM:
					pop(stack);
					push(stack, NON_TERM, Cond_);
					push(stack, NON_TERM, Expr);
				break;
			default :
				printf("Cond\n");
				return 1;
			}
		}
		break;

		case Cond_:
		{
			switch(input){
				case quality:
					pop(stack);
					push(stack, NON_TERM, Expr);
					push(stack, TERMINAL, quality);
				break;
				case same:
					pop(stack);
					push(stack, NON_TERM, Expr);
					push(stack, TERMINAL, same);
				break;
			default :
				printf("Cond_\n");
				return 1;
			}
		}
		break;

		case Term:
		{
			switch(input){
				case WORD:
				case NUM:
					pop(stack);
					push(stack, NON_TERM, Term_);
					push(stack, NON_TERM, Fact);
				break;
			default :
				printf("Term\n");
				return 1;
			}
		}
		break;

		case Term_:
		{
			switch(input){
				case mul:
					pop(stack);
					push(stack, NON_TERM, Fact);
					push(stack, TERMINAL, mul);
				break;
				case add:
				case L_Brace:
				case colon:
				case same:
				case quality:
				case THEN:
					pop(stack);
				break;
			default :
				printf("Term_\n");
				return 1;
			}
		}
		break;

		case Expr:
		{
			switch(input){
				case WORD:
				case NUM:
					pop(stack);
					push(stack, NON_TERM, Expr_);
					push(stack, NON_TERM, Term);
				break;
			default :
				printf("Expr\n");
				return 1;
			}
		}
		break;

		case Expr_:
		{
			switch(input){
				case add:
					pop(stack);
					push(stack, NON_TERM, Term);
					push(stack, TERMINAL, add);
					break;
				case L_Brace:
				case colon:
				case same:
				case quality:
				case THEN:
					pop(stack);
					break;
			default :
				printf("Expr_\n");
				return 1;
			}
		}
		break;

		case Fact:
		{
			switch(input){
				case WORD:
					pop(stack);
					push(stack, TERMINAL, WORD);
					break;
				case NUM:
					pop(stack);
					push(stack, TERMINAL, NUM);
					break;
			default :
				printf("Fact\n");
				return 1;
			}
		}
		break;

		case Vtype:
		{
			switch(input){
				case INT:
					pop(stack);
					push(stack, TERMINAL, INT);
					break;
				case CHAR:
					pop(stack);
					push(stack, TERMINAL, CHAR);
					break;
			default :
				printf("Vtype\n");
				return 1;
			}
		}
		break;

		default :
			return 1;
	}
}


