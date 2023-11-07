#include <stdbool.h>
#include "machine_types.h"
#include "file_location.h"
#include "parser_types.h"
#include "parser.h"

// Declaration Checking Tree Walk
// scope_check_program(progast) to check for duplicate declarations
extern void scope_check_program(block_t prog);
extern void scope_check_block(block_t blk);
extern void scope_check_const_decls(const_decls_t cds);
extern void scope_check_var_decls(var_decls_t var);
extern void scope_check_proc_decls(proc_decls_t proc);