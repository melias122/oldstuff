%option noyywrap
%{

	typedef enum {
		begin,
		END
	} Keyword;

// begin, end, if, then, while, do, procedure, function, Integer, Real, Boolean, type, var, array, of, record
	#define KW_BEGIN 1
	#define KW_END 2
	#define KW_IF 3
	#define KW_THEN 4
	#define KW_WHILE 5
	#define KW_DO 6
	#define KW_PROCEDURE 7
	#define KW_FUNCTION 8
	#define KW_INTEGER 9
	#define KW_REAL 10
	#define KW_BOOLEAN 11
	#define KW_TYPE 12
	#define KW_VAR 13
	#define KW_ARRAY 14
	#define KW_OF 15
	#define KW_RECORD 16

// +, −, ∗, /, mod, and, or, not, <, <=, >, >=, =, <>, :=
	#define OP_ADD 17
	#define OP_SUB 18
	#define OP_MUL 19
	#define OP_DIV 20
	#define OP_MOD 21
	#define OP_AND 22
	#define OP_OR 23
	#define OP_NOT 24
	#define OP_LT 25
	#define OP_LTE 26
	#define OP_GT 27
	#define OP_GTE 28
	#define OP_EQ 29
	#define OP_CO 30
	#define OP_AS 31

// (, ), [, ], . ., ., „ ;, :
	#define SEP_LRB
	#define SEP_RRB
	#define SEP_LSB
	#define SEP_RSB
	#define SEP_DOT // .
	#define SEP_COMMA // ,
	#define SEP_SEMICOLON // ;
	#define SEP_COLON // :

// 	DIGIT [0-9]

// [a-zA-Z][a-zA-Z0-9]* // identificator
// [+-]*[0]*{DIGIT}+ 		// int const
// [+-]*[0]*{DIGIT}+"."{DIGIT}+ // real const
// true
// false
// [ \t\n\r\f]
	
//	int typ = 0;
//	void type(int t);

%}

%%

begin { return begin; }
end { return KW_END; }
if { return KW_IF; }
then { return KW_THEN; }
while { return KW_WHILE; }
do { return KW_DO; }
procedure { return KW_PROCEDURE; }
function { return KW_FUNCTION; }
Integer { return KW_INTEGER; }
Real { return KW_REAL; }
Boolean { return KW_BOOLEAN; }
type { return KW_TYPE; }
var { return KW_VAR; }
array { return KW_ARRAY; }
of { return KW_OF; }
record { return KW_RECORD; }

"+" { return OP_ADD; }
− { return OP_SUB; }
∗ { return OP_MUL; }
"/" { return OP_DIV; }
mod { return OP_MOD; }
and { return OP_AND; }
or { return OP_OR; }
not { return OP_NOT; }
"<" { return OP_LT; }
"<=" { return OP_LTE; }
">" { return OP_GT; }
">=" { return OP_GTE; }
"=" { return OP_EQ; }
"<>" { return OP_CO; }
":=" { return OP_AS; }

"("
")" 
"[" 
"]" 
".."
"."
";"
":"

%%

int main(void) {

	int code = yylex();
	printf("%d\n", code);

	return 0;
}
