#include <stdbool.h>
#include "machine_types.h"
#include "file_location.h"
#include "parser_types.h"
#include "parser.h"
// Declaration Checking Tree Walk
// scope_check_program(progast) to check for duplicate declarations
extern void scope_check_program(block_t prog);
extern void scope_check_block(block_t blk);
void scope_check_const_decls(const_decls_t cds);
void scope_check_const_defs(const_defs_t cds);
void scope_check_constDef(const_def_t def);
void scope_check_declare_ident(ident_t id, AST_type type);

void scope_check_varDecls(var_decls_t vds);
void scope_check_varDecl(var_decl_t vd);
void scope_check_idents(idents_t ident, AST_type type);

void scope_check_proc_decls(proc_decls_t pro);
void scope_check_procDecl(proc_decl_t dcl);

void scope_check_stmt(stmt_t stmt);
void scope_check_assignStmt(assign_stmt_t stmt);
void scope_check_callStmt(call_stmt_t call);
void scope_check_beginStmt(begin_stmt_t stmt);
void scope_check_stmts(stmts_t stmts);
void scope_check_ifStmt(if_stmt_t stmt);
void scope_check_whileStmt(while_stmt_t stmt);

void scope_check_condition(condition_t cond);
void scope_check_oddCond(odd_condition_t cond);
void scope_check_relOpCond(rel_op_condition_t cond);

void scope_check_readStmt(read_stmt_t stmt);
void scope_check_writeStmt(write_stmt_t stmt);
void scope_check_skipStmt(skip_stmt_t stmt);

void scope_check_expr(expr_t exp);
void scope_check_binary_op_expr(binary_op_expr_t exp);
void scope_check_ident_expr(ident_t id);

id_use *scope_check_ident_declared(file_location floc, const char *name);
