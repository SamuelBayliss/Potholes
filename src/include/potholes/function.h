/* 
 * File:   function.h
 * Author: sb1708
 *
 * Created on July 22, 2013, 4:18 PM
 */

#ifndef FUNCTION_H
#define	FUNCTION_H

#ifdef	__cplusplus
extern "C" {
#endif

    enum pth_ast_node_param_type {pth_param, pth_expr, pth_mem}
    
   struct pth_ast_node_param { 
       pth_ast_node_param_type type;
       union { 
           int param;
           pet_expr * expr;
       };
   };
   
    
   struct pth_ast_node_declare {
       pet_scop * scop;
       int n_params;
       pth_ast_node_params * params;
   };

   struct pth_ast_node_call {
       struct pth_ast_node_declare * declaration;
       const char ** names;
   };
   


   
#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTION_H */

