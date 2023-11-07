#ifndef SYMTAB_H
#define SYMTAB_H
#include <stdbool.h>
#include "id_use.h"
#include "scope.h"
#include "id_attrs.h"

// To build symbol table
void symtab_initialize();
unsigned int symtab_size();
bool symtab_empty();
unsigned int symtab_scope_loc_count();
unsigned int symtab_scope_size();
bool symtab_scope_full();
unsigned int symtab_current_nesting_level();
bool symtab_full();
bool symtab_declared(const char *name);
id_use *symtab_lookup(const char *name);
void symtab_insert(const char *name, id_attrs *attrs);
static void add_ident(scope_t *s, const char *name, id_attrs *attrs);
bool symtab_declared_in_current_scope(const char *name);
bool symtab_declared(const char *name);

void symtab_enter_scope();
void symtab_leave_scope();

#endif
