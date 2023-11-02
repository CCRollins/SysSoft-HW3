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

program: block {setProgAST($1);}

block: constDecls varDecls procDecls stmt {$$ = block_t($1, $2, $3, $4);};

constDecls: constDecl {$$ = const_decls_t($1);};

constDecl: constsym constDefs {$$ = const_decl_t($2);};

constDefs: constDef {$$ = const_defs_t($1);}
| constDefs commasym constDef {$$ = const_defs_t($1, $3);};

constDef: identsym eqsym numbersym {$$ = const_def_t($1, $3);};

varDecls: varDecl {$$ = var_decls_t($1);};

varDecl: varsym idents {$$ = var_decl_t($2);};

idents: identsym {$$ = idents_t($1);};

procDecls: procDecl {$$ = proc_decls_t($1);};

procDecl: proceduresym identsym block {$$ = proc_decl_t($1, $2, $3);};

stmt: assignStmt {$$ = stmt_t($1);}
| callStmt {$$ = stmt_t($1);}
| beginStmt {$$ = stmt_t($1);}
| ifStmt {$$ = stmt_t($1);}
| whileStmt {$$ = stmt_t($1);}
| readStmt {$$ = stmt_t($1);}
| writeStmt {$$ = stmt_t($1);}
| skipStmt {$$ = stmt_t($1)};

assignStmt: identsym becomessym expr {$$ = assign_stmt_t($1, $3);};
stmts: stmt {$$ = stmts_t($1);};
callStmt: callsym identsym {$$ = call_stmt_t($2);};
beginStmt: beginsym stmts endsym {$$ = begin_stmt_t($2);};
ifStmt: ifsym condition thensym stmt elsesym stmt endsym {$$ = if_stmt_t($2, $4, $6);};
whileStmt: whilesym condition dosym stmt endsym {$$ = while_stmt_t($2, $4);};
readStmt: readsym identsym {$$ = read_stmt_t($2);};
writeStmt: writesym expr {$$ = write_stmt_t($2);};
skipStmt: skipsym {$$ = skip_stmt_t();};

condition: oddCondition {$$ = condition_t($1);} 
| expr relOp expr {$$ = condition_t($1, $3, $1);};

oddCondition: oddsym expr {$$ = odd_condition_t($1);};

expr: expr relOpCondition expr {$$ = expr_t($1, $2, $1);}
| identsym {$$ = expr_t($1);}
| numbersym {$$ = expr_t($1);};

relOpCondition: plussym {$$ = binary_op_expr_t($1);}
| minussym {$$ = binary_op_expr_t($1);}
| multsym {$$ = binary_op_expr_t($1);}
| divsym {$$ = binary_op_expr_t($1);};

empty: empty

relOp:
term:
factor: 
posSign:

%%

// Set the program's ast to be ast
void setProgAST(block_t ast) {progast = ast;}
