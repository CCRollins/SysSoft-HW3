#include <stdio.h>
#include "scope_check.h"
#include "symtab.h"
void scope_check_program(block_t prog)
{
    scope_check_block(prog);
}

// create sym table
void scope_check_block(block_t blk)
{
    symtab_enter_scope();
    scope_check_constDecls(blk.const_decls);
    scope_check_varDecls(blk.var_decls);
    scope_check_procDecls(blk.proc_decls);
    scope_check_stmt(blk.stmt);
    symtab_leave_scope();
}

void scope_check_const_decls(const_decls_t cds)
{
    const_decl_t *dcl = cds.const_decls;
    while (dcl != NULL)
    {
        scope_check_constDecl(*dcl);
        dcl = dcl->next;
    }
}

// build symbol table

int main()
{
    return 0;
}