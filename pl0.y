 /* $Id: bison_pl0_y_top.y,v 1.1 2023/10/19 18:47:38 leavens Exp $ */
 /* This file should be named pl0.y, it won't work with other file names! */

%code top {
#include <stdio.h>
}

%code requires {

 /* Including "ast.h" must be at the top, to define the AST type */
#include "ast.h"
#include "machine_types.h"
#include "parser_types.h"
#include "lexer.h"

    /* Report an error to the user on stderr */
extern void yyerror(const char *filename, const char *msg);

}    /* end of %code requires */

%verbose
%define parse.lac full
%define parse.error detailed

 /* the following passes file_name to yyerror,
    and declares it as an formal parameter of yyparse. */
%parse-param { char const *file_name }

%token <ident> identsym
%token <number> numbersym
%token <token> plussym    "+"
%token <token> minussym   "-"
%token <token> multsym    "*"
%token <token> divsym     "/"

%token <token> periodsym  "."
%token <token> semisym    ";"
%token <token> eqsym      "="
%token <token> commasym   ","
%token <token> becomessym ":="

%token <token> constsym   "const"
%token <token> varsym     "var"
%token <token> proceduresym "procedure"
%token <token> callsym    "call"
%token <token> beginsym   "begin"
%token <token> endsym     "end"
%token <token> ifsym      "if"
%token <token> thensym    "then"
%token <token> elsesym    "else"
%token <token> whilesym   "while"
%token <token> dosym      "do"
%token <token> readsym    "read"
%token <token> writesym   "write"
%token <token> skipsym    "skip"
%token <token> oddsym     "odd"

%token <token> neqsym     "<>"
%token <token> ltsym      "<"
%token <token> leqsym     "<="
%token <token> gtsym      ">"
%token <token> geqsym     ">="
%token <token> lparensym  "("
%token <token> rparensym  ")"

%type <block> program
%type <block> block
%type <const_decls> constDecls

%type <var_decls> varDecls
%type <var_decl> varDecl
%type <idents> idents
%type <proc_decls> procDecls
%type <empty> empty
%type <const_decl> constDecl
%type <const_def> constDef
%type <const_defs> constDefs

%type <proc_decl> procDecl
%type <stmt> stmt
%type <assign_stmt> assignStmt
%type <call_stmt> callStmt
%type <begin_stmt> beginStmt
%type <if_stmt> ifStmt
%type <while_stmt> whileStmt
%type <read_stmt> readStmt
%type <write_stmt> writeStmt
%type <skip_stmt> skipStmt
%type <stmts> stmts
%type <condition> condition
%type <odd_condition> oddCondition
%type <rel_op_condition> relOpCondition
%type <expr> expr

%type <token> relOp
%type <expr> term
%type <expr> factor
%type <token> posSign

%start program

%code {
 /* extern declarations provided by the lexer */
extern int yylex(void);

 /* The AST for the program, set by the semantic action 
    for the nonterminal program. */
block_t progast; 

 /* Set the program's ast to be t */
extern void setProgAST(block_t t);
}

%%
 /* Write your grammar rules below and before the next %% */

program : block ;

block : const_decls var_decls proc_decls stmt ".end"
          { parseProgram(
                       ast_block($1,
                                   $2,
				   $3, $4
                                  )
                       );
          }
          ;

constDef : "const" identsym "=" numbersym { $$ = const_def($2,$4); };
const_decls : { const_decl } ;
const_decl : const_defs ;
const_defs : const_def | const_defs "," const_def ;

var_decl : idents ;
var_decls : { var_decl } ;
idents : ident | idents , ident ;

proc_decls : { proc_decl } ;
proc_decl : ident ; block ;

stmt : assign_stmt | call_stmt | begin_stmt | if_stmt | while_stmt | read_stmt | write_stmt | skip_stmt ;
assign_stmt : ident : expr
call_stmt : call ident ;
begin-stmt : begin stmt end ;
if_stmt : if condition then stmt else stmt ;
while_stmt : while condition do stmt ; 
read_stmt : read ident
write_stmt : write expr
skip_stmt : skip_stmt
stmts : stmt | stmts ; stmt ;

condition : odd_condition | rel_op_condition ;
odd_condition : odd expr ;
rel_op_condition : expr relOp expr ;
relOp : + | <> | < | <= | > | >= ;

expr : term | expr plussym term | expr minussym term ;
term : factor | term multsym factor | term divsym factor ;
factor : ident | minussym number | posSign number | ( expr ) ;
posSign : plussym | empty ;
empty



label : identsym ;

%%

// Set the program's ast to be ast
void parseProgram(block_t ast) { progast = ast; }
