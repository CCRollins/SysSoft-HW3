#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "id_attrs.h"
#include "id_use.h"
#include "file_location.h"
#include "ast.h"
#include "utilities.h"
#include "scope.h"
#include "symtab.h"
#include "scope_check.h"

void scope_check_program(block_t prog)
{
    scope_check_block(prog);
}

// create sym table
void scope_check_block(block_t blk)
{
    symtab_enter_scope();
    scope_check_const_decls(blk.const_decls);
    scope_check_varDecls(blk.var_decls);
    scope_check_proc_decls(blk.proc_decls);
    scope_check_stmt(blk.stmt);
    symtab_leave_scope();
}

// Consts
void scope_check_const_decls(const_decls_t cds)
{
    const_decl_t *dcl = cds.const_decls;
    while (dcl != NULL)
    {
        scope_check_const_defs(dcl->const_defs);
        dcl = dcl->next;
    }
}

void scope_check_const_defs(const_defs_t cds)
{
    const_def_t *def = cds.const_defs;
    while (def != NULL)
    {
        scope_check_constDef(*def);
        def = def->next;
    }
}

void scope_check_constDef(const_def_t def)
{
    scope_check_declare_ident(def.ident, def.type_tag);
}

void scope_check_declare_ident(ident_t id, AST_type type)
{
    if (symtab_declared_in_current_scope(id.name))
    {
        id_use *getOg = symtab_lookup(id.name);
        id_kind newKind = 0;
        switch (type)
        {
        case 0:
            break;
        case 1:
            newKind = constant_idk;
            break;
        case 2:
            newKind = variable_idk;
            break;
        case 3:
            newKind = procedure_idk;
            break;
        case 4:
            newKind = constant_idk;
            break;
        case 5:
            newKind = constant_idk;
            break;
        case 6:
            newKind = constant_idk;
            break;
        case 7:
            newKind = variable_idk;
            break;
        case 8:
            break;
        case 9:
            newKind = procedure_idk;
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
        case 16:
            break;
        case 17:
            break;
        case 18:
            break;
        case 19:
            break;
        case 20:
            break;
        case 21:
            break;
        case 22:
            break;
        case 23:
            break;
        case 24:
            break;
        case 25:
            break;
        case 26:
            break;
        case 27:
            break;
        case 28:
            break;
        }
        bail_with_prog_error(*(id.file_loc), "%s \"%s\" is already declared as a %s", kind2str(newKind), id.name, kind2str(getOg->attrs->kind));
    }
    else
    {
        int ofst_cnt = symtab_scope_loc_count();
        id_attrs *attrs = create_id_attrs(*(id.file_loc), (id_kind)type, ofst_cnt);
        switch (type)
        {
        case 0:
            break;
        case 1:
            attrs->kind = constant_idk;
            break;
        case 2:
            attrs->kind = variable_idk;
            break;
        case 3:
            attrs->kind = procedure_idk;
            break;
        case 4:
            attrs->kind = constant_idk;
            break;
        case 5:
            attrs->kind = constant_idk;
            break;
        case 6:
            attrs->kind = constant_idk;
            break;
        case 7:
            attrs->kind = variable_idk;
            break;
        case 8:
            break;
        case 9:
            attrs->kind = procedure_idk;
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
        case 16:
            break;
        case 17:
            break;
        case 18:
            break;
        case 19:
            break;
        case 20:
            break;
        case 21:
            break;
        case 22:
            break;
        case 23:
            break;
        case 24:
            break;
        case 25:
            break;
        case 26:
            break;
        case 27:
            break;
        case 28:
            break;
        }

        symtab_insert(id.name, attrs);
    }
}

// Var Decls
void scope_check_varDecls(var_decls_t vds)
{
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL)
    {
        scope_check_varDecl(*vdp);
        vdp = vdp->next;
    }
}

void scope_check_varDecl(var_decl_t vd)
{
    scope_check_idents(vd.idents, vd.type_tag);
}

void scope_check_idents(idents_t ident, AST_type type)
{
    ident_t *idp = ident.idents;
    while (idp != NULL)
    {
        scope_check_declare_ident(*idp, type);
        idp = idp->next;
    }
}

// Proc
void scope_check_proc_decls(proc_decls_t pro)
{
    proc_decl_t *dcl = pro.proc_decls;
    while (dcl != NULL)
    {
        scope_check_procDecl(*dcl);
        dcl = dcl->next;
    }
}

void scope_check_procDecl(proc_decl_t dcl)
{
    // scope_check_declare_ident_def(def.ident, def.type_tag);
    scope_check_declare_ident(ast_ident(dcl.file_loc, dcl.name), dcl.type_tag);

    scope_check_block(*dcl.block);
}

// stmts
void scope_check_stmt(stmt_t stmt)
{
    switch (stmt.stmt_kind)
    {
    case assign_stmt:
        scope_check_assignStmt(stmt.data.assign_stmt);
        break;
    case call_stmt:
        scope_check_callStmt(stmt.data.call_stmt);
        break;
    case begin_stmt:
        scope_check_beginStmt(stmt.data.begin_stmt);
        break;
    case if_stmt:
        scope_check_ifStmt(stmt.data.if_stmt);
        break;
    case while_stmt:
        scope_check_whileStmt(stmt.data.while_stmt);
        break;
    case read_stmt:
        scope_check_readStmt(stmt.data.read_stmt);
        break;
    case write_stmt:
        scope_check_writeStmt(stmt.data.write_stmt);
        break;
    case skip_stmt:
        scope_check_skipStmt(stmt.data.skip_stmt);
        break;
    default:
        bail_with_error("Call to scope_check_stmt with an AST that is not a statement!");
        break;
    }
}

// assign statement
void scope_check_assignStmt(assign_stmt_t stmt)
{
    const char *name = stmt.name;
    id_use *idu = scope_check_ident_declared(*(stmt.file_loc), name);
    assert(idu != NULL);
    scope_check_expr(*(stmt.expr));
}

// call statement
void scope_check_callStmt(call_stmt_t call)
{
}

// begin statement
void scope_check_beginStmt(begin_stmt_t stmt)
{
    symtab_enter_scope();
    scope_check_stmts(stmt.stmts);
    symtab_leave_scope();
}

void scope_check_stmts(stmts_t stmts)
{
    stmt_t *sp = stmts.stmts;
    while (sp != NULL)
    {
        scope_check_stmt(*sp);
        sp = sp->next;
    }
}

// if statement
void scope_check_ifStmt(if_stmt_t stmt)
{
    scope_check_condition(stmt.condition);
    scope_check_stmt(*(stmt.then_stmt));
    scope_check_stmt(*(stmt.then_stmt));
}

// while statement
void scope_check_whileStmt(while_stmt_t stmt)
{
    scope_check_condition(stmt.condition);
    scope_check_stmt(*(stmt.body));
}

void scope_check_condition(condition_t cond)
{
    scope_check_oddCond(cond.data.odd_cond);
    scope_check_relOpCond(cond.data.rel_op_cond);
}

void scope_check_oddCond(odd_condition_t cond)
{
    scope_check_expr(cond.expr);
}

void scope_check_relOpCond(rel_op_condition_t cond)
{
    scope_check_expr(cond.expr1);
    scope_check_expr(cond.expr2);
}

// read statement
void scope_check_readStmt(read_stmt_t stmt)
{
    scope_check_ident_declared(*(stmt.file_loc), stmt.name);
}

// write statement
void scope_check_writeStmt(write_stmt_t stmt)
{
    scope_check_expr(stmt.expr);
}

// skip statement
void scope_check_skipStmt(skip_stmt_t stmt)
{
}

void scope_check_expr(expr_t exp)
{
    switch (exp.expr_kind)
    {
    case expr_bin:
        scope_check_binary_op_expr(exp.data.binary);
        break;
    case expr_ident:
        scope_check_ident_expr(exp.data.ident);
        break;
    case expr_number:
        // no identifiers are possible in this case, so just return
        break;
    default:
        bail_with_error("Unexpected expr_kind_e (%d) in scope_check_expr",
                        exp.expr_kind);
        break;
    }
}

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

id_use *scope_check_ident_declared(file_location floc, const char *name)
{
    id_use *ret = symtab_lookup(name);
    if (ret == NULL)
    {
        bail_with_prog_error(floc,
                             "identifier \"%s\" is not declared!",
                             name);
    }
    return ret;
}
