#define NON_TERM 999
#define TERMINAL 998

typedef struct stackElement{
	int type;				//NON_TERM or TERMINAL
	int nonterm;			//IF NONTERM
	struct node* terminal;	//IF TERMNINAL
	struct stackElement* next;	//NEXT
}Element;

typedef struct stackElement* STACK;

#define Prog	101
#define Word	102
#define Block	103
#define Decls	104
#define Decls_	105
#define Decl	106
#define Words	107
#define Words_	108
#define Vtype	109
#define Slist	110
#define Slist_	111
#define Stat	112
#define Cond	113
#define Cond_	114
#define Expr	115
#define Expr_	116
#define Term	117
#define Term_	118
#define Fact	119
