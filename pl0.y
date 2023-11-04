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
%parse-param {char const *file_name}

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
%type <assign_stmt_t> assignStmt
%type <call_stmt_t> callStmt
%type <begin_stmt_t> beginStmt
%type <if_stmt_t> ifStmt
%type <while_stmt_t> whileStmt
%type <read_stmt_t> readStmt
%type <write_stmt_t> writeStmt
%type <skip_stmt_t> skipStmt
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

program: block {setProgAST(ast_program($1));};

block: constDecls varDecls procDecls stmt {$$ = ast_block($1, $2, $3, $4);};

constDecls: empty {$$ = ast_const_decls_empty();} | constDecl {$$ = ast_const_decls($1);};

constDecl: constsym constDefs {$$ = ast_const_decl($2);};

constDefs: constDef {$$ = ast_const_defs($1);}
| constDefs commasym constDef {$$ = ast_const_defs($1, $3);};

constDef: identsym eqsym numbersym {$$ = ast_const_def($1, $3);};

varDecls: empty {$$ = ast_var_decls_empty();} | varDecl {$$ = ast_var_decls($1);};

varDecl: varsym idents {$$ = ast_var_decl($2);};

idents: identsym {$$ = ast_idents($1);};

procDecls: empty {$$ = ast_proc_decls_empty();} | procDecl {$$ = ast_proc_decls($1);};

procDecl: proceduresym identsym block {$$ = ast_proc_decl($1, $2, $3);};

stmt: assignStmt {$$ = ast_stmt($1);}
| callStmt {$$ = ast_stmt($1);}
| beginStmt {$$ = ast_stmt($1);}
| ifStmt {$$ = ast_stmt($1);}
| whileStmt {$$ = ast_stmt($1);}
| readStmt {$$ = ast_stmt($1);}
| writeStmt {$$ = ast_stmt($1);}
| skipStmt {$$ = ast_stmt($1)};

assignStmt: identsym becomessym expr {$$ = ast_assign_stmt($1, $3);};
callStmt: callsym identsym {$$ = ast_call_stmt($2);};
beginStmt: beginsym stmts endsym {$$ = ast_begin_stmt($2);};
ifStmt: ifsym condition thensym stmt elsesym stmt endsym {$$ = ast_if_stmt($2, $4, $6);};
whileStmt: whilesym condition dosym stmt endsym {$$ = ast_while_stmt($2, $4);};
readStmt: readsym identsym {$$ = ast_read_stmt($2);};
writeStmt: writesym expr {$$ = ast_write_stmt($2);};
skipStmt: skipsym {$$ = ast_skip_stmt();};

stmts: stmt {$$ = ast_stmts($1);};

condition: oddCondition {$$ = ast_condition($1);} 
| relOpCondition { $$ = ast_condition_rel($1); } ;

oddCondition: oddsym expr {$$ = ast_odd_condition($1);};

expr: expr relOpCondition expr {$$ = ast_expr($1, $2, $1);}
| identsym {$$ = ast_expr($1);}
| numbersym {$$ = ast_expr($1);};

relOpCondition: expr relOp expr { $$ =  ast_rel_op_condition($1, $2, $1); } ;

relOp: eqsym | neqsym | ltsym | leqsym | gtsym | geqsym;

empty: %empty
{ file_location *floc
= file_location_make(lexer_filename(), lexer_line());
$$ = ast_empty(floc);};

posSign: plussym;

%%

// Set the program's ast to be ast
void setProgAST(block_t ast) {progast = ast;}
