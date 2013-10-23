/* 
 * File:   statement.h
 * Author: sb1708
 *
 * Created on August 1, 2013, 4:17 PM
 */

#ifndef STATEMENT_H
#define	STATEMENT_H

#ifdef	__cplusplus
//extern "C" {
#endif

#include <isl/printer.h>
#include <pet.h>
#include <isl/ast_build.h>

isl_printer * pth_print_pet_stmt_type(isl_printer * printer, isl_ast_build * build, pet_expr * expr);
isl_printer * pth_print_pet_expr(isl_printer * printer, isl_ast_build * build,  pet_scop * scop, pet_stmt * stmt, pet_expr *);
isl_id * pth_initialize_memory_space_id(isl_ctx * ctx, const char * name);
isl_id * pth_memory_space_id();
#ifdef	__cplusplus
//}
#endif

#endif	/* STATEMENT_H */

