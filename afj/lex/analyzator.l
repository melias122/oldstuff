%option noyywrap
%{

	typedef enum {

		constant = 1,
		identificator = 2,
		any = 3,

		// constant_typ
		constant_int = 10,
		constant_real = 11,
		constant_bool = 12,

		kw_begin = 100,
		kw_end,
		kw_if,
		kw_then,
		kw_while,
		kw_do,
		kw_procedure,
		kw_function,
		kw_Integer,
		kw_Real,
		kw_Boolean,
		kw_type,
		kw_var,
		kw_array,
		kw_of,
		kw_record,

		op_add = 200,
		op_sub,
		op_mul,
		op_div,
		op_mod,
		op_and,
		op_or,
		op_not,
		op_lt,
		op_lte,
		op_gt,
		op_gte,
		op_eq,
		op_lg,
		op_as,

		sep_lrb = 300,
		sep_rrb,
		sep_lsb,
		sep_rsb,
		sep_dotdot,
		sep_dot,
		sep_comma,
		sep_semicolon,
		sep_colon,

	} myLang;
	
	int ctyp = 0; // ctyp == 0, if no constant is setb
	void set_ctyp(int t);

%}

%%

\s 				{} // [ \t\n\r\f]
(?i:begin) 		{ return kw_begin; }
(?i:end) 		{ return kw_end; }
(?i:if) 		{ return kw_if; }
(?i:then) 		{ return kw_then; }
(?i:while) 		{ return kw_while; }
(?i:do) 		{ return kw_do; }
(?i:procedure) 	{ return kw_procedure; }
(?i:function) 	{ return kw_function; }
(?i:Integer) 	{ return kw_Integer; }
(?i:Real) 		{ return kw_Real; }
(?i:Boolean) 	{ return kw_Boolean; }
(?i:type) 		{ return kw_type; }
(?i:var) 		{ return kw_var; }
(?i:array) 		{ return kw_array; }
(?i:of) 		{ return kw_of; }
(?i:record) 	{ return kw_record; }

"+" 			{ return op_add; }
"−" 			{ return op_sub; }
"∗" 			{ return op_mul; }
"/" 			{ return op_div; }
(?i:mod) 		{ return op_mod; }
(?i:and)		{ return op_and; }
(?i:or)			{ return op_or; }
(?i:not)		{ return op_not; }
"<" 			{ return op_lt; }
"<=" 			{ return op_lte; }
">" 			{ return op_gt; }
">=" 			{ return op_gte; }
"=" 			{ return op_eq; }
"<>" 			{ return op_lg; }
":=" 			{ return op_as; }

"("				{ return sep_lrb; }
")"				{ return sep_rrb; }
"["				{ return sep_lsb; }
"]"				{ return sep_rsb; }
".."			{ return sep_dotdot; }
"."				{ return sep_dot; }
","				{ return sep_comma; }
";"				{ return sep_semicolon; }
":"				{ return sep_colon; }

[+-]?[0]*[0-9]+					{ set_ctyp(constant_int); return constant; }
[+-]?[0]*[0-9]+"."[0]*[0-9]+	{ set_ctyp(constant_real); return constant; }
(?i:true|false) 				{ set_ctyp(constant_bool); return constant; }

[a-zA-Z][a-zA-Z0-9]* 			{ return identificator; }
. 								{ return any; }

%%

void set_ctyp(int t) {
	ctyp = t;
}

int main(void) {

	int code;

	while ((code = yylex()) > 0) {
		if (ctyp > 0) {
			printf("%s %d %d\n", yytext, code, ctyp);
		} else {
			printf("%s %d\n", yytext, code);
		}
		ctyp = 0;
	}

	return 0;
}
