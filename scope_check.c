/* $Id: scope_check.c,v 1.9 2023/11/01 13:20:24 leavens Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scope_check.h"
#include "id_attrs.h"
#include "file_location.h"
#include "ast.h"
#include "utilities.h"
#include "symtab.h"
#include "scope_check.h"

// Build the symbol table for the given program AST
// and check the given program AST for duplicate declarations
// or uses of identifiers that were not declared
void scope_check_program(program_t prog)
{
    symtab_enter_scope();
    scope_check_varDecls(prog.var_decls);
    scope_check_stmt(prog.stmt);
    symtab_leave_scope();
}

// build the symbol table and check the declarations in vds
void scope_check_varDecls(var_decls_t vds)
{
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL) {
	scope_check_varDecl(*vdp);
	vdp = vdp->next;
    }
}

// Add declarations to the current scope's symbol table for vd,
// producing errors for duplicate declarations
void scope_check_varDecl(var_decl_t vd)
{
    scope_check_idents(vd.idents, vd.var_type);

}

// Add declarations for the names in ids to the current scope's symbol table,
// for variables of the type vt,
// producing errors for any duplicate declarations
void scope_check_idents(idents_t ids, var_type_e vt)
{
    ident_t *idp = ids.idents;
    while (idp != NULL) {
	scope_check_declare_ident(*idp, vt);
	idp = idp->next;
    }
}

// Add a declaration of the name id.name with the type vt
// to the current scope's symbol table,
// producing an error if this would be a duplicate declaration
void scope_check_declare_ident(ident_t id, var_type_e vt)
{
    if (symtab_defined_in_current_scope(id.name)) {
	bail_with_prog_error(*(id.file_loc),
			     "Variable \"%s\" has already been declared!",
			     id.name);
    } else {
	int ofst_cnt = symtab_scope_loc_count();
	id_attrs *attrs = id_attrs_loc_create(*(id.file_loc),
					      vt, ofst_cnt);
	symtab_insert(id.name, attrs);
    }
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_stmt(stmt_t stmt)
{
    switch (stmt.stmt_kind) {
    case assign_stmt:
	scope_check_assignStmt(stmt.data.assign_stmt);
	break;
    case begin_stmt:
	scope_check_beginStmt(stmt.data.begin_stmt);
	break;
    case if_stmt:
	scope_check_ifStmt(stmt.data.if_stmt);
	break;
    case read_stmt:
	scope_check_readStmt(stmt.data.read_stmt);
	break;
    case write_stmt:
	scope_check_writeStmt(stmt.data.write_stmt);
	break;
    default:
	bail_with_error("Call to scope_check_stmt with an AST that is not a statement!");
	break;
    }
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_assignStmt(assign_stmt_t stmt)
{
    const char *name = stmt.name;
    id_use *idu = scope_check_ident_declared(*(stmt.file_loc), name);
    assert(idu != NULL);
    scope_check_expr(*(stmt.expr));
}

// check the statement to make sure that
// there are no duplicate declarations and that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_beginStmt(begin_stmt_t stmt)
{
    symtab_enter_scope();
    scope_check_varDecls(stmt.var_decls);
    scope_check_stmts(stmt.stmts);
    symtab_leave_scope();
}

// check the statements to make sure that
// all idenfifiers referenced in them have been declared
// (if not, then produce an error)
void scope_check_stmts(stmts_t stmts)
{
    stmt_t *sp = stmts.stmts;
    while (sp != NULL) {
	scope_check_stmt(*sp);
	sp = sp->next;
    }
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_ifStmt(if_stmt_t stmt)
{
    scope_check_expr(stmt.expr);
    scope_check_stmt(*(stmt.body));
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_readStmt(read_stmt_t stmt)
{
    scope_check_ident_declared(*(stmt.file_loc), stmt.name);
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_writeStmt(write_stmt_t stmt)
{
    scope_check_expr(stmt.expr);
}

// check the expresion to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_expr(expr_t exp)
{
    switch (exp.expr_kind) {
    case expr_bin_op:
	scope_check_binary_op_expr(exp.data.binary);
	break;
    case expr_ident:
	scope_check_ident_expr(exp.data.ident);
	break;
    case expr_number:
	// no identifiers are possible in this case, so just return
	break;
    case expr_logical_not:
	scope_check_expr(*(exp.data.logical_not));
	break;
    default:
	bail_with_error("Unexpected expr_kind_e (%d) in scope_check_expr",
			exp.expr_kind);
	break;
    }
}

// check the expression (exp) to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
void scope_check_binary_op_expr(binary_op_expr_t exp)
{
    scope_check_expr(*(exp.expr1));
    // (note: no identifiers can occur in the operator)
    scope_check_expr(*(exp.expr2));
}

// check the identifier (id) to make sure that
// all it has been declared (if not, then produce an error)
void scope_check_ident_expr(ident_t id)
{
    scope_check_ident_declared(*(id.file_loc), id.name);
}

// check that the given name has been declared,
// if so, then return an id_use (containing the attributes) for that name,
// otherwise, produce an error using the file_location (floc) given.
id_use *scope_check_ident_declared(file_location floc, const char *name)
{
    id_use *ret = symtab_lookup(name);
    if (ret == NULL) {
	bail_with_prog_error(floc,
				"identifier \"%s\" is not declared!",
				name);
    }
    return ret;
}