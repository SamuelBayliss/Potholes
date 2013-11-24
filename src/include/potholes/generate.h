/* 
 * File:   codegen.h
 * Author: sb1708
 *
 * Created on July 22, 2013, 5:07 PM
 */

#ifndef CODEGEN_H
#define	CODEGEN_H

#ifdef	__cplusplus
//extern "C" {
#endif

#include <vector>
#include <map>
#include <pet.h>
/* Appropriate Codegen module */

#include <isl/ast.h>
#include <isl/ast_build.h>

#include <string>

#include <potholes/ast.h>

typedef std::string TypeName;
typedef std::string VarName;
typedef std::map<VarName, TypeName> VarMap;

  

  

std::string pth_generate_scop_name(pet_scop * scop);
VarMap pth_generate_scop_function_prototype_arguments(pet_scop * scop); 
std::vector<std::string> pth_generate_scop_function_invocation_arguments(pet_scop * scop);   
std::string pth_generate_scop_function_invocation(pet_scop * scop, std::string name);
std::string pth_generate_scop_function_declaration(pet_scop * scop, std::string name);
  void pth_generate_initialize(isl_ctx * );      

pth_stmt * pth_get_scop_statement_by_id(pth_scop *, pth_id *);
pth_stmt * pth_get_scop_statement_by_name(pth_scop *, pth_id *);
   

pth_ast_stmt * pth_generate_ast_stmt(pth_ast_build *, pth_scop *, pth_id *); // scop has an id from pet domain
pth_ast_stmt * pth_generate_ast_stmt_call(pth_ast_build *, pth_scop *, pth_id * id);
pth_ast_stmt * pth_generate_ast_stmt_assign(pth_ast_build *, pth_scop *, pth_id * id);      

isl_ast_expr * pth_generate_ast_expr(pth_ast_build *, pth_scop *, pth_stmt *, pth_expr *);
isl_ast_expr * pth_generate_ast_expr_binary(pth_ast_build*, pth_scop*, pth_stmt*, pth_expr*);
isl_ast_expr * pth_generate_ast_expr_arith(pth_ast_build*, pth_scop*, pth_stmt*, pth_expr*);
isl_ast_expr * pth_generate_ast_expr_relational(pth_ast_build*, pth_scop*, pth_stmt*, pth_expr*);
isl_ast_expr * pth_generate_ast_expr_unary(pth_ast_build*, pth_scop*, pth_stmt*, pth_expr*);
isl_ast_expr * pth_generate_ast_expr_access(pth_ast_build*, pth_scop*, pth_stmt*, pth_expr*);
isl_ast_expr * pth_generate_ast_expr_call(pth_ast_build*, pth_scop*, pth_stmt*, pth_expr*);


  
#ifdef	__cplusplus
//}
#endif

#endif	/* CODEGEN_H */

