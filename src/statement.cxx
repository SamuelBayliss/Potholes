#include <potholes/statement.h>
#include <potholes/affine.h>

#include <cassert>
#include <iostream>
#include <isl/aff.h>
#include <isl/ilp.h>

#include <isl/ast_build.h>



isl_id * memory_space_id = NULL;


isl_id * pth_initialize_memory_space_id(isl_ctx * ctx, const char * name) { 
    memory_space_id = isl_id_alloc(ctx, name, NULL);
    return memory_space_id;
}
isl_id * pth_memory_space_id() { 
    return memory_space_id;
}
isl_printer * pth_print_pet_expr(isl_printer * printer, isl_ast_build * build, pet_scop * scop , pet_stmt * stmt, pet_expr * expr);

isl_printer * pth_print_pet_stmt_brackets(isl_printer * printer,isl_ast_build * build,  pet_scop * scop, pet_stmt * stmt, pet_expr * expr) {
  
    printer = isl_printer_print_str(printer, "(");
    printer = pth_print_pet_expr(printer, build, scop, stmt, expr);
    printer = isl_printer_print_str(printer, ")");
    return printer;
}
isl_printer * pth_print_pet_star_assign(isl_printer * printer, isl_ast_build * build, pet_scop * scop, pet_stmt * stmt, pet_expr * expr, pet_op_type op) { 
    // print LHS

    
    assert(expr->n_arg == 2);
    pet_expr * lhs = expr->args[0];
    printer = pth_print_pet_stmt_brackets(printer, build, scop, stmt, lhs);
    
    switch(op) {
        case pet_op_add_assign:  {
            printer = isl_printer_print_str(printer, "+=");
        } break;
	case pet_op_sub_assign: {
            printer = isl_printer_print_str(printer, "-=");
        } break;
	case pet_op_mul_assign: {
            printer = isl_printer_print_str(printer, "*=");
        } break;
	case pet_op_div_assign: { 
           printer = isl_printer_print_str(printer, "/=");
        } break;
	case pet_op_assign: {
            printer = isl_printer_print_str(printer, "="); 
         } break;
        default :  assert(false); 
        
    };
    // print RHS
       pet_expr * rhs = expr->args[1];
       printer = pth_print_pet_stmt_brackets(printer, build, scop, stmt, rhs);

   return printer; 
}
isl_printer * pth_print_pet_star_arith(isl_printer * printer, isl_ast_build * build, pet_scop * scop, pet_stmt * stmt,  pet_expr * expr, pet_op_type op) {
    
    assert(expr->n_arg == 2);
    pet_expr * lhs = expr->args[0];
    printer = pth_print_pet_stmt_brackets(printer, build, scop, stmt, lhs);
    
    switch(op) {
        case pet_op_add:  {
            printer = isl_printer_print_str(printer, "+");
        } break;
	case pet_op_sub: {
            printer = isl_printer_print_str(printer, "-");
        } break;
	case pet_op_mul: {
            printer = isl_printer_print_str(printer, "*");
        } break;
	case pet_op_div: { 
           printer = isl_printer_print_str(printer, "/");
        } break;
	case pet_op_mod: {
            printer = isl_printer_print_str(printer, "%"); 
         } break;
        default :  assert(false); 
        
    };
    // print RHS
       pet_expr * rhs = expr->args[1];
       printer = pth_print_pet_stmt_brackets(printer, build, scop, stmt, rhs);
    
    
    
       return printer; 
}
isl_printer * pth_print_pet_star_relational(isl_printer * printer, isl_ast_build * build, pet_scop * scop, pet_stmt *, pet_expr * expr, pet_op_type op) { 
       return printer; 
}
isl_printer * isl_print_pet_unary_minus(isl_printer *printer, isl_ast_build * build, pet_scop * scop, pet_stmt *, pet_expr * expr, pet_op_type op) {
    return printer;
}

isl_printer * isl_printer_pet_inc_dec(isl_printer *printer, isl_ast_build * build, pet_scop * scop, pet_stmt *, pet_expr * expr, pet_op_type op) {
    return printer;
}

isl_printer * pth_print_pet_expr_binary (isl_printer * printer, isl_ast_build * build, pet_scop * scop, pet_stmt * stmt, pet_expr * expr) {
    pet_op_type op = expr->op;
    switch(op) {
    	case pet_op_add_assign:  
	case pet_op_sub_assign: 
	case pet_op_mul_assign: 
	case pet_op_div_assign: 
	case pet_op_assign: {  printer = pth_print_pet_star_assign(printer, build, scop, stmt, expr, op); } break;
	case pet_op_add: 
	case pet_op_sub: 
	case pet_op_mul: 
	case pet_op_div: 
	case pet_op_mod: { printer = pth_print_pet_star_arith(printer, build, scop, stmt, expr, op); } break;
	case pet_op_eq: 
	case pet_op_le: 
	case pet_op_lt: ;
	case pet_op_gt: { printer = pth_print_pet_star_relational(printer, build, scop, stmt, expr, op); } break;
	case pet_op_minus: { printer = isl_print_pet_unary_minus(printer, build, scop, stmt, expr, op); } break;
	case pet_op_post_inc: 
	case pet_op_post_dec:
	case pet_op_pre_inc: 
	case pet_op_pre_dec: { printer = isl_printer_pet_inc_dec(printer, build, scop, stmt, expr, op);  } break;
	case pet_op_address_of: { assert(false); } break;
        case pet_op_kill : { assert(false); } break;
        case pet_op_last : { assert(false); } break;
        default : { assert(false); } break;
    };
    
    return printer;
}

isl_printer * pth_print_pet_stmt_type(isl_printer * printer, isl_ast_build * build, pet_scop * scop, pet_stmt * stmt, pet_expr * expr) { 
    pet_expr_type type = expr->type;
    
    switch(type) { 
        case pet_expr_access : { 
                printer = isl_printer_print_str(printer, "expr_access_operation\n");
        } break;
	case pet_expr_call : { 
                printer = isl_printer_print_str(printer, "expr_call_operation\n");
        } break;
	case pet_expr_cast : { 
                printer = isl_printer_print_str(printer, "expr_cast_operation\n");
        } break;
	case pet_expr_double : {
                printer = isl_printer_print_str(printer, "expr_double_operation\n");
        } break;
	case pet_expr_unary : {
                printer = isl_printer_print_str(printer, "expr_unary_operation\n");
        } break;
	case pet_expr_binary : { 
                printer = pth_print_pet_expr_binary(printer, build, scop, stmt, expr);
                
        } break;
	case pet_expr_ternary : { 
                printer = isl_printer_print_str(printer, "expr_ternary_operation\n");
        } break;
        default: { assert(false);} break;
        }
    
    return printer;    
    }



/*isl_ast_expr * pth_get_ast_expr_from_aff(isl_aff *) {
    return NULL;
}

isl_ast_expr * pth_subst_expr(isl_ast_expr * expr) {
    return expr;
}
*/

void print_pa(isl_pw_aff * pa) {
     isl_space * space = isl_pw_aff_get_space(pa);
     isl_local_space * local_space = isl_local_space_from_space(space);
     isl_printer * printer = isl_printer_to_str(isl_local_space_get_ctx(local_space));
     printer = isl_printer_print_pw_aff(printer, pa);
     std::cerr << isl_printer_get_str(printer) <<"\n";
     isl_printer_free(printer);
}
void print_map(isl_map * pa) {
     isl_space * space = isl_map_get_space(pa);
     isl_local_space * local_space = isl_local_space_from_space(space);
     isl_printer * printer = isl_printer_to_str(isl_local_space_get_ctx(local_space));
     printer = isl_printer_print_map(printer, pa);
     std::cerr << isl_printer_get_str(printer) <<"\n";
     isl_printer_free(printer);
}

void print_set(isl_set * pa) {
     isl_space * space = isl_set_get_space(pa);
     isl_local_space * local_space = isl_local_space_from_space(space);
     isl_printer * printer = isl_printer_to_str(isl_local_space_get_ctx(local_space));
     printer = isl_printer_print_set(printer, pa);
     std::cerr << isl_printer_get_str(printer) <<"\n";
     isl_printer_free(printer);
}
void print_basic_map(isl_basic_map * pa) {
     isl_space * space = isl_basic_map_get_space(pa);
     isl_local_space * local_space = isl_local_space_from_space(space);
     isl_printer * printer = isl_printer_to_str(isl_local_space_get_ctx(local_space));
     printer = isl_printer_print_basic_map(printer, pa);
     std::cerr << isl_printer_get_str(printer) <<"\n";
     isl_printer_free(printer);
}

void print_multi_pw_aff(isl_multi_pw_aff * pa) {
     isl_space * space = isl_multi_pw_aff_get_space(pa);
     isl_local_space * local_space = isl_local_space_from_space(space);
     isl_printer * printer = isl_printer_to_str(isl_local_space_get_ctx(local_space));
     printer = isl_printer_print_multi_pw_aff(printer, pa);
     std::cerr << isl_printer_get_str(printer) <<"\n";
     isl_printer_free(printer);
}

 /*static isl_multi_pw_aff *parametrize_nested_exprs(
            isl_multi_pw_aff *index)
   {
           int i;
           isl_ctx *ctx;
           isl_space *space, *space2;
           isl_local_space *ls;
           isl_multi_aff *ma, *ma2;
   
           ctx = isl_multi_pw_aff_get_ctx(index);
           space = isl_multi_pw_aff_get_domain_space(index);
           space = isl_space_unwrap(space);
   
           space2 = isl_space_domain(isl_space_copy(space));
           ma = isl_multi_aff_identity(isl_space_map_from_set(space2));
   
           space = isl_space_insert_dims(space, isl_dim_param, 0, 2);
           //for (i = 0; i < expr->n_arg; ++i) {
           //       isl_id *id = isl_id_alloc(ctx, NULL, expr->args[i]);
   
           //        space = isl_space_set_dim_id(space, isl_dim_param, i, id);
           //}
         space2 = isl_space_domain(isl_space_copy(space));
         ls = isl_local_space_from_space(space2);
          ma2 = isl_multi_aff_zero(space);
      for (i = 0; i < 2; ++i) {
                  isl_aff *aff;
                  aff = isl_aff_var_on_domain(isl_local_space_copy(ls),
                                                  isl_dim_param, i);
                 ma2 = isl_multi_aff_set_aff(ma2, i, aff);
          }
          isl_local_space_free(ls);
  
          ma = isl_multi_aff_range_product(ma, ma2);
  
          return isl_multi_pw_aff_pullback_multi_aff(index, ma);
 }*/







isl_printer * pth_print_pet_expr_access(isl_printer * printer, isl_ast_build * build, pet_scop * scop, pet_stmt * stmt, pet_expr * expr) { 
     printer = isl_printer_print_str(printer, "access");
     
    // isl_ast_expr * access = pth_generate_access_expr(build, scop, stmt, expr);
     
     // Find appropriate pet array
     
     // Synthesize Flattening Function
     
     // Apply Flattening Function
  
    
     
    // std::cerr << isl_printer_get_str(mprinter) << "\n";
 
     
       //        printer = isl_printer_print_str(printer, isl_printer_get_str(mprinter));
           //    isl_printer_flush(mprinter);
    // isl_id * id = isl_space_get_tuple_id(space, isl_dim_out);
     
     //printer = isl_printer_print_id(printer, id);
     
   //  printer = isl_printer_print_str(printer, isl_space_get_tuple_name(space, isl_dim_out));
     
     return printer;
}

isl_printer * pth_print_pet_expr(isl_printer * printer, isl_ast_build * build,  pet_scop * scop, pet_stmt * stmt, pet_expr * expr) { 
 
    pet_expr_type type = expr->type;
    
    switch(type) { 
        case pet_expr_access : { 
            printer = pth_print_pet_expr_access(printer, build, scop, stmt, expr);
        } break;
	case pet_expr_call : { 
               assert(false); 
        } break;
	case pet_expr_cast : { 
                assert(false);
        } break;
	case pet_expr_double : {
                assert(false);
        } break;
	case pet_expr_unary : {
                 assert(false);
        } break;
	case pet_expr_binary : { 
               printer = pth_print_pet_expr_binary(printer, build, scop, stmt, expr);
                
        } break;
	case pet_expr_ternary : { 
               assert(false);
        } break;
        default: { assert(false); } break;
        }
    
    return printer;    
    }
