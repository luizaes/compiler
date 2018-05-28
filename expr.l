delim	[ \t]
ws	{delim}+
digito	[0-9]
caracterMinusculo [a-z]
simbolo [-_]
caracterMaiusculo [A-Z]
numInt	{digito}{digito}*
numFloat ({digito}+\.{digito}*)|({digito}*\.{digito}+)
literal \".*\"
id	(({caracterMinusculo}+)|({caracterMaiusculo}+))({digito}*|{simbolo}*|{caracterMaiusculo}*|{caracterMinusculo}*)*

%%
{ws}	{}
"\n"	{linha++;}
"+"	{return TADD;}
"-"	{return TSUB;}
"*"	{return TMUL;}
"/"	{return TDIV;}
"("	{return TAPAR;}
")"	{return TFPAR;}
"{"	{return TACH;}
"}"	{return TFCH;}
";"	{return TPONTVIR;}
"="	{return TATRI;}
"<"	{return TMENOR;}
">"	{return TMAIOR;}
"&"	{return TE;}
"!"	{return TNEG;}
"|"	{return TOU;}
","	{return TVIR;}
"while" {return TWHILE;}
"if"	{return TIF;}
"else"	{return TELSE;}
"print"	{return TPRINT;}
"read"	{return TREAD;}
"int"	{return TINT;}
"String" {return TSTRING;}
"float"	{return TFLOAT;}
"return" {return TRETURN;}
"void"  {return TVOID;}
{literal} {strncpy(yylval.string, yytext, 10000); return TLITERAL;}
{id}	{strncpy(yylval.id, yytext, MAX_ID-1); return TID;}
{numFloat} {yylval.numFloat = atof(yytext); return TNUMFLOAT;} 
{numInt} {yylval.numInt = atoi(yytext); return TNUMINT;} 
. {printf("ERROR: Erro léxico na linha %d.\n", linha);}
