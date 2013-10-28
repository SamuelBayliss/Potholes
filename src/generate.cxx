
/*
 
 Clang frontend puts SCOP into function call, adds global extern void * axi_mem;
  
 and emits code to .potholes/autoesl/code.c
 
 
 this code is compiled in auto-esl to give verilog file  
 
 #pragma potholes fpga
 
 Clang frontend adds pragma annotation to function call indicating co-synthesis
 
 
 #pragma potholes scope main
 #pragma potholes interface sdram
 
 Clang frontend adds pragma annotation to axi_mem indicating which data layout to provide transparent access to. 
 Clang frontend adds pragma annotation to axi_mem indicating which memory interface to use for data transfer
 
 LLVM pass then puts all the code inside an function initialize (called from simulator)
 
 LLVM pass registers appropriate functions for co-simulation of memories
 
 LLVM generates top level verilog with 
  
 $potholes_transfer_control(label)
  
 elaboration time function call creates appropriate call-backs for  
 
 simulator code then runs and hits $potholes_transfer_control("initialize") whereupon it calls into 
 global lookup table in llvm. 
  
 looks up program counter and restores stack from stack_pointer
 
 all memory lookups go through fpga memory layout and are dereferenced using the module memory layout. 
 
 when the verilog module completes, it should call
  
 $potholes_transfer_control(label) to transfer control back to the llvm simulator
  
 
 
 
 */

#include <potholes/generate.h>
#include <potholes/statement.h>


#include <potholes/ast.h>

#include <potholes/affine.h>
#include <potholes/parallel.h>

#include <sstream>
#include <iostream>
#include <cassert>

#include <cstring>

#include <isl/ast_build.h>
#include <isl/schedule.h>
#include <isl/flow.h>

void pth_generate_initialize(isl_ctx * ctx) { 
    pth_initialize_memory_space_id(ctx, "mem");
}


std::string pth_generate_scop_name(pet_scop * scop) {
    std::stringstream ss;
    ss << "accelerated_scop" ; 
    return ss.str();
}



std::string pth_generate_scop_function_invocation(pet_scop * scop) {
    std::stringstream ss;
    
     isl_id * mem = pth_memory_space_id();
    
     ss << "double *" << " " << isl_id_get_name(mem) << ";" << "\n";
     
     std::vector<std::string> invocation_arguments = pth_generate_scop_function_invocation_arguments(scop);
     
     std::vector<std::string>::iterator argits = invocation_arguments.begin();
     
     while(argits != invocation_arguments.end()) {
         if (argits->find("_offset") != std::string::npos) { 
          ss << "unsigned" << " " << *argits << ";" << "\n";
         }
        argits++;
    //    if (argits != invocation_arguments.end()) ss << ",";
    }
     
    ss << pth_generate_scop_name(scop) << "(";
   
            
     argits = invocation_arguments.begin();
    
    while(argits != invocation_arguments.end()) {
        ss << *argits;
        argits++;
        if (argits != invocation_arguments.end()) ss << ",";
    }
     ss << ");";
    
    return ss.str();
}


VarMap pth_generate_scop_function_prototype_arguments(pet_scop * scop) {
    // pass parameters by value
    VarMap vm;
    
    isl_set * context = scop->context;

    isl_space * space = isl_set_get_space(context);
   
  /*  isl_local_space * lspace = isl_local_space_from_space(space);

    isl_printer * printer = isl_printer_to_str(isl_local_space_get_ctx(lspace));
        printer = isl_printer_print_set(printer, array->extent);
        std::cout << isl_printer_get_str(printer) << "\n";
        printer = isl_printer_flush(printer);
    */    
    isl_id * mem = pth_memory_space_id();
    
   vm.insert(std::pair<std::string, std::string>(std::string(isl_id_get_name(mem)), "double *"));
    
    // pass scalars by value
    for (unsigned i = 0 ; i < isl_space_dim(space, isl_dim_param) ; i++ ) {
        const char * pname = isl_space_get_dim_name(space, isl_dim_param, i);
          vm.insert(std::pair<std::string, std::string>(pname, "unsigned"));
    }
    
    // pass vectors by pointer
    for (int j = 0 ; j < scop->n_array  ; j++ ) {
        pet_array * array = scop->arrays[j];
        std::string element_type = array->element_type;
        std::string pname = isl_set_get_tuple_name(array->extent) + std::string("_offset");
        std::string ptype = std::string("unsigned");
        
    
       vm.insert(std::pair<std::string, std::string>(pname, ptype));
        
    }

   
    
 

    return vm;
}

std::vector<std::string> pth_generate_scop_function_invocation_arguments(pet_scop * scop) {
    VarMap arguments = pth_generate_scop_function_prototype_arguments(scop);
    VarMap::iterator argits = arguments.begin();
    
    std::vector<std::string> variables;
    
 
    
    while(argits != arguments.end()) {
     variables.push_back(argits->first); 
     argits++;
    }
    
    return variables;
} 


isl_ctx * pth_get_ctx_from_scop(pet_scop * scop) { 
      isl_set * context = scop->context;

    isl_space * space = isl_set_get_space(context);
   
    isl_local_space * lspace = isl_local_space_from_space(space);
    return isl_local_space_get_ctx(lspace);
    
   
}


isl_printer * pth_get_printer_from_scop(pet_scop * scop) {
    isl_ctx * ctx = pth_get_ctx_from_scop(scop);
     isl_printer * printer = isl_printer_to_str(ctx);
     printer = isl_printer_set_output_format(printer, ISL_FORMAT_ISL);
     return printer;
}


isl_printer * pth_get_pretty_printer_from_scop(pet_scop * scop) {
    isl_ctx * ctx = pth_get_ctx_from_scop(scop);
     isl_printer * printer = isl_printer_to_str(ctx);
     printer = isl_printer_set_output_format(printer, ISL_FORMAT_C);
     return printer;
}

pth_stmt * pth_get_scop_statement_by_name(pth_scop * scop, pth_id * id) {
    const char * sname = isl_id_get_name(id);
    
    // go through all the scop names
    
    for (int i = 0 ; i < scop->scop->n_stmt ; i++ ) {
        pth_stmt * stmt = scop->scop->stmts[i];
        isl_set * domain = stmt->domain;
        isl_space * space = isl_set_get_space(domain);
        const char * dname = isl_space_get_tuple_name(space, isl_dim_out);
        if (strcmp(dname, sname) == 0) { 
            return stmt;
        }
        
    }
    assert(false);
    return NULL;
}

pth_stmt * pth_get_scop_statement_by_id(pth_scop * scop, pth_id * ast_statement_id) {

    for (unsigned i = 0 ; i < scop->n_stmt ; i++ ) {
        pth_stmt * stmt = scop->scop->stmts[i];
        isl_set * domain =  stmt->domain;
        isl_space * space = isl_set_get_space(domain);
        if (ast_statement_id == isl_space_get_tuple_id(space, isl_dim_out)) {
            return stmt;
        }
    }
return NULL;  
};

pth_ast_stmt * pth_ast_get_scop_statement_by_id(pth_scop * scop, pth_id * ast_statement_id) {
    for (unsigned i = 0 ; i < scop->n_stmt ; i++ ) {
        pth_ast_stmt * stmt = scop->stmts[i];
        if (ast_statement_id == stmt->id) {
            return stmt;
        }
    }
return NULL;  
}




pth_ast_stmt * pth_generate_ast_stmt_assign(pth_ast_build * build, pth_scop * scop, pth_id * stmt_id) {

   pet_stmt * stmt = pth_get_scop_statement_by_name(scop, stmt_id);
   pet_expr * expr = stmt->body;
     
   pth_ast_stmt * output = pth_ast_stmt_alloc(pth_assign_stmt);
   
   output->id = stmt_id;
   
   pth_expr * lhs_expr = expr->args[pet_bin_lhs];
   pth_expr * rhs_expr = expr->args[pet_bin_rhs];
   
  isl_ast_expr * lhs = pth_generate_ast_expr(build, scop, stmt, lhs_expr); 
  isl_ast_expr * rhs = pth_generate_ast_expr(build, scop, stmt, rhs_expr);
   
  assert(lhs);
  assert(rhs);
  
   switch(lhs_expr->type) {
       
       case pet_expr_access : {
          output->assign.lhs = pth_ast_node_from_isl_ast_node(isl_ast_node_alloc_user(lhs));
       }; break;
       default : assert(false);
   }
  
    switch(expr->op) { 
        case pet_op_add_assign: {
     
                rhs = pth_ast_expr_to_isl_ast_expr(pth_ast_expr_alloc_binary(isl_ast_op_add, lhs, rhs)); 
        } break;
	case pet_op_sub_assign: {
            rhs =  pth_ast_expr_to_isl_ast_expr(pth_ast_expr_alloc_binary(isl_ast_op_add, lhs, rhs)); 
        } break;
	case pet_op_mul_assign: {
            rhs = pth_ast_expr_to_isl_ast_expr(pth_ast_expr_alloc_binary(isl_ast_op_add, lhs, rhs)); 
        } break;
	case pet_op_div_assign: {
                assert(false);
                } break;
	default : {  }
   }
    
    
   switch(rhs_expr->type) { 
       case pet_expr_access : { 
          output->assign.rhs = pth_ast_node_from_isl_ast_node(isl_ast_node_alloc_user(rhs));
       }; break;
       case pet_expr_double : { 
               
           assert(false);
       }; break;
       case pet_expr_binary : { 
          output->assign.rhs = pth_ast_node_from_isl_ast_node(isl_ast_node_alloc_user(rhs));
       } break;
       case pet_expr_ternary : { 
             
           assert(false);
       }; break;
       case pet_expr_unary : {    
         output->assign.rhs = pth_ast_node_from_isl_ast_node(isl_ast_node_alloc_user(rhs));
      
       }; break;
   
       case pet_expr_call : { 
         output->assign.rhs = pth_ast_node_from_isl_ast_node(isl_ast_node_alloc_user(rhs));
        
       }; break;
       case pet_expr_cast : { 
           assert(false);
       }
    
   }
   
  
   
   
  return output;
}
pth_ast_stmt * pth_generate_ast_stmt_call(pth_ast_build * build, pth_scop * scop, pth_id * stmt_id) {
      pet_stmt * stmt = pth_get_scop_statement_by_name(scop, stmt_id);
      
      isl_ctx * ctx = pth_ast_build_get_ctx(build);

      pth_ast_expr * call_expr = pth_ast_expr_alloc_op(ctx, isl_ast_op_call, stmt->body->n_arg);
    
      pth_ast_stmt * output = pth_ast_stmt_alloc(pth_call_stmt);
      output->call = pth_ast_node_from_isl_ast_node(isl_ast_node_alloc_user(pth_ast_expr_to_isl_ast_expr(call_expr)));
      output->id = stmt_id;
      return output;
}


isl_ast_expr * pth_generate_ast_expr_arith(pth_ast_build * build, pth_scop * scop, pet_stmt * stmt, pet_expr * expr, pet_op_type op) {
    pth_ast_expr * output;
//    isl_ctx * ctx = pth_ast_build_get_ctx(build);
    
        
    isl_ast_expr * lhs = pth_generate_ast_expr(build, scop, stmt, expr->args[pet_bin_lhs]);
    isl_ast_expr * rhs = pth_generate_ast_expr(build, scop, stmt, expr->args[pet_bin_rhs]);
    
    switch(op) {
        case pet_op_add: output = pth_ast_expr_alloc_binary(isl_ast_op_add, lhs, rhs); break;
	case pet_op_sub: output = pth_ast_expr_alloc_binary(isl_ast_op_sub, lhs, rhs); break;
	case pet_op_mul: output = pth_ast_expr_alloc_binary(isl_ast_op_mul, lhs, rhs); break;
	case pet_op_div: output = pth_ast_expr_alloc_binary(isl_ast_op_div, lhs, rhs); break;
	case pet_op_mod: assert(false); break;
        default : { 
            assert(false);
        }
    }
    
    return pth_ast_expr_to_isl_ast_expr(output);
}
isl_ast_expr * pth_generate_ast_expr_relational(pth_ast_build * build, pth_scop * scop, pet_stmt * stmt, pet_expr * expr, pet_op_type op){
    pth_ast_expr * output;

    assert(false);
    isl_ctx * ctx = pth_ast_build_get_ctx(build);
         switch(op) {
                case pet_op_eq: output = pth_ast_expr_alloc_op(ctx, isl_ast_op_eq, 2); break;
                case pet_op_le: output = pth_ast_expr_alloc_op(ctx, isl_ast_op_le, 2); break;
                case pet_op_lt: output = pth_ast_expr_alloc_op(ctx, isl_ast_op_lt, 2); break;
                case pet_op_gt: output = pth_ast_expr_alloc_op(ctx, isl_ast_op_gt, 2); break;
             default : { 
                 assert(false);
             }
         }
    return pth_ast_expr_to_isl_ast_expr(output);
}
isl_ast_expr * pth_generate_ast_expr_unary(pth_ast_build * build, pth_scop * scop, pet_stmt * stmt, pet_expr * expr, pet_op_type op) {
   pth_ast_expr * output;  
   assert(false);
   isl_ctx * ctx = pth_ast_build_get_ctx(build);
   
      switch(op) {
                case pet_op_minus : output = pth_ast_expr_alloc_op(ctx, isl_ast_op_eq, 1); break;
                case pet_op_post_inc : assert(false); break;
                case pet_op_post_dec : assert(false); break;
                case pet_op_pre_inc : assert(false); break;
                case pet_op_pre_dec : assert(false); break;
             default : { 
                 assert(false);
             }
      }
    return pth_ast_expr_to_isl_ast_expr(output);
}

isl_ast_expr * pth_generate_ast_expr_access(pth_ast_build * build, pth_scop * scop, pth_stmt * stmt, pth_expr * expr) {

   isl_ast_expr * access_expr = pth_generate_access_expr(build, scop, stmt, expr);
   assert(access_expr);
   return access_expr;
}
isl_ast_expr * pth_generate_ast_expr_binary(pth_ast_build * build, pth_scop * scop, pet_stmt * stmt, pet_expr * expr) {
    
    isl_ast_expr * output_expr = NULL;
    
  
    
     pet_op_type op = expr->op;
    switch(op) {
    	case pet_op_add_assign:  
	case pet_op_sub_assign: 
	case pet_op_mul_assign: 
	case pet_op_div_assign: 
	case pet_op_assign: {  
            assert(false);
        } break;
	case pet_op_add: 
	case pet_op_sub: 
	case pet_op_mul: 
	case pet_op_div: 
	case pet_op_mod: { 
                output_expr = pth_generate_ast_expr_arith(build, scop, stmt, expr, op); 
                assert(output_expr);
                } break;
	case pet_op_eq: 
	case pet_op_le: 
	case pet_op_lt: ;
	case pet_op_gt: { output_expr = pth_generate_ast_expr_relational(build, scop, stmt, expr, op);
                          assert(output_expr);
                } break;
	case pet_op_minus: 
	case pet_op_post_inc: 
	case pet_op_post_dec:
	case pet_op_pre_inc: 
	case pet_op_pre_dec: { output_expr = pth_generate_ast_expr_unary(build, scop, stmt, expr, op);  assert(output_expr);  } break;
	case pet_op_address_of: { assert(false); } break;
        case pet_op_kill : { assert(false); } break;
        case pet_op_last : { assert(false); } break;
        default : { assert(false); } break;
    }
    
    return output_expr;
}

isl_ast_expr * pth_generate_ast_expr(pth_ast_build * build, pth_scop * scop, pth_stmt * stmt, pth_expr * expr){
      pet_expr_type type = expr->type;
      
      isl_ast_expr * output = NULL;
      
    switch(type) { 
        case pet_expr_access : { 
            output = pth_generate_ast_expr_access(build, scop, stmt, expr);
            assert(output);
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
               output = pth_generate_ast_expr_binary(build, scop, stmt, expr); 
                assert(output);
        } break;
	case pet_expr_ternary : { 
               assert(false);
        } break;
        default: { assert(false); } break;
        }
    
    assert(output);
    return output;
}

pth_ast_stmt * pth_ast_stmt_alloc(pth_stmt_type type) { 
    
    pth_ast_stmt * stmt = (pth_ast_stmt *)malloc(sizeof(pth_ast_stmt));
    stmt->type = type;
 return stmt;   
}

pth_ast_stmt * pth_generate_ast_stmt(pth_ast_build * build, pth_scop * scop, pth_id * id) {
         pet_stmt * stmt = pth_get_scop_statement_by_name(scop, id);
        
        if (stmt) {  
            switch(stmt->body->type) {
                case pet_expr_call : { 
                   return pth_generate_ast_stmt_call(build, scop, id);
                } ; break;
                case pet_expr_binary : { 
                    switch(stmt->body->op) {
                 	case pet_op_add_assign:  
                        case pet_op_sub_assign: 
                        case pet_op_mul_assign: 
                        case pet_op_div_assign: 
                        case pet_op_assign: {  
                                return  pth_generate_ast_stmt_assign(build, scop, id);
                        } break;
                        default : assert(false);
                    }
                       
                } break;
           
            default : {
                        assert(false);
            } ;
        }
  
                 
        }
         assert(false);
         return NULL;
}



typedef struct { 
    isl_ast_build * build;
    pet_scop * scop;
} pth_scop_build;


int extract_statement(isl_map *map, void *user) { 
    isl_space * space = isl_map_get_space(map);
    isl_id ** tuple_id = (isl_id **)(user);
    *tuple_id = isl_id_copy(isl_space_get_tuple_id(space, isl_dim_out));
    return 1;
}


isl_ast_node * pth_generate_user_statement(isl_ast_build * build, void * user) { 
    
    pth_scop * scop = (pth_scop *)(user);

    

    isl_ast_node * node = NULL;
    
    pth_ast_build * pbuild = pth_ast_build_from_isl_ast_build(build);
    

    int map_count = isl_union_map_n_map(pbuild->executed);
    
    if (map_count == 1) {
        isl_id * tuple_id;
        int success = isl_union_map_foreach_map(pbuild->executed,extract_statement,&tuple_id);
        (void)(success);
            pth_ast_stmt * stmt = pth_generate_ast_stmt(pth_ast_build_from_isl_ast_build(build), scop, tuple_id);
            pth_scop_insert_stmt(scop, stmt);
            isl_val * val = isl_val_zero(pth_ast_build_get_ctx(pbuild));
            isl_ast_node * node = isl_ast_node_alloc_user(isl_ast_expr_from_val(val));  
            isl_ast_node_set_annotation(node, tuple_id);
            return node;
    }
    
   

    assert(false);

    return node;
}


isl_printer * pth_print_call_statement(isl_printer * printer, isl_ast_print_options * options, pth_scop * scop, pth_ast_stmt * stmt) { 
    assert(stmt->type == pth_call_stmt);
    
    printer = isl_printer_print_ast_node(printer, pth_ast_node_to_isl_ast_node(stmt->call));
    assert(printer != NULL);
    return printer;   
}
isl_printer * pth_print_assign_statement(isl_printer * printer, isl_ast_print_options * options, pth_scop * scop, pth_ast_stmt * stmt) {
    assert(stmt->type == pth_assign_stmt);

    
    isl_ast_expr * lhs_expr = isl_ast_node_user_get_expr(pth_ast_node_to_isl_ast_node(stmt->assign.lhs));
    
    pth_ast_expr * pth_expr = pth_ast_expr_from_isl_ast_expr(lhs_expr);
     
         printer = isl_printer_start_line(printer);
           
    if (pth_expr->type == isl_ast_expr_id) {
        
        pth_id_annotation * annotation = (pth_id_annotation *)isl_id_get_user(pth_array_offset_lookup(scop->array_offsets, pth_expr->u.id));
       
        if (annotation && annotation->first_use == pth_expr) {
           
            printer = isl_printer_print_str(printer, pth_supported_type_to_str(annotation->type));
            printer = isl_printer_print_str(printer, " ");
        } 
    }
           
    printer = isl_printer_print_ast_expr(printer, lhs_expr);
         assert(printer != NULL);

    printer = isl_printer_print_str(printer, " = ");

    printer = isl_printer_print_ast_expr(printer, isl_ast_node_user_get_expr(pth_ast_node_to_isl_ast_node(stmt->assign.rhs)));
    printer = isl_printer_print_str(printer, ";");
    
         printer = isl_printer_end_line(printer);
         
      assert(printer != NULL);
    return printer;
}

isl_printer * pth_print_user_statement(isl_printer * printer, isl_ast_print_options * options, isl_ast_node * node, void * user) {
    
     
   
    pth_scop * scop = (pth_scop *)(user);
    
    // get annotation from node
   pth_id  * statement_id = isl_ast_node_get_annotation(node);
    
    pth_ast_stmt * stmt = pth_ast_get_scop_statement_by_id(scop, statement_id);
    // look up pth stmt from scop
  
    if (stmt) { 
    
    switch(stmt->type) {
        case pth_call_stmt : { 
           printer = pth_print_call_statement(printer, options, scop, stmt);
        } break;
        case pth_assign_stmt : {
              printer = pth_print_assign_statement(printer, options,scop,  stmt);
        } break;
        default : assert(false);
    }
    }

    return printer;
}


pth_scop * pth_scop_populate_array_offsets(pth_scop * pscop) {
    
    pet_scop * scop = pscop->scop;
    isl_ctx * ctx = isl_set_get_ctx(scop->context);
    for (int i = 0; i < scop->n_array; i++) {
        pet_array * arr = scop->arrays[i];
        isl_space * space = isl_set_get_space(arr->extent);
        assert(isl_space_has_tuple_id(space, isl_dim_out));
        isl_id * id = isl_space_get_tuple_id(space, isl_dim_out);
        assert(isl_space_has_tuple_name(space, isl_dim_out));
        const char * tuple_name = isl_space_get_tuple_name(space, isl_dim_out);
        const char * tuple_suffix = "_offset";
        
        char * offset_name = (char *)(malloc(sizeof(char) * (strlen(tuple_name) + strlen(tuple_suffix) + 1)));
        strcpy(offset_name, tuple_name);
        strcat(offset_name, tuple_suffix);
        
        pth_id_annotation * annotation = pth_id_annotation_alloc(pth_type_pointer_to_double);

        isl_id * offset_id = isl_id_alloc(ctx, offset_name, (void *)annotation);
     
        pscop->array_offsets = pth_array_offset_insert(pscop->array_offsets, id, offset_id);
    }
    return pscop;
}

isl_ast_node_list * pth_scop_populate_array_definitions(pth_scop * scop) {
        isl_ast_node_list * list = isl_ast_node_list_alloc(isl_set_get_ctx(scop->scop->context), scop->scop->n_array);
         isl_ast_expr * mem_expression = isl_ast_expr_from_id(isl_id_copy(pth_memory_space_id()));
        for (int i = 0; i < scop->scop->n_array; i++) {
            
            pet_array * arr = scop->scop->arrays[i];
            isl_space * space = isl_set_get_space(arr->extent);
        
  
            isl_id * id = isl_space_get_tuple_id(space, isl_dim_out);
           

            // build definition expression     
            isl_ast_expr * definition_expression = isl_ast_expr_from_id(isl_id_copy(id));
              isl_ast_node *  definition = isl_ast_node_alloc_user(definition_expression);
            definition = isl_ast_node_set_annotation(definition, (isl_id *)(isl_id_copy(id)));
            list  = isl_ast_node_list_add(list, definition);
            
            // set first use
            
            pth_id * offset_id = pth_array_offset_lookup(scop->array_offsets, id);
             pth_id_annotation * annotation = (pth_id_annotation *)isl_id_get_user(offset_id);
            
            
            
            isl_ast_expr * offset_expr = isl_ast_expr_from_id(isl_id_copy(offset_id));
             pth_id_annotation_set_first_use(annotation, pth_ast_expr_from_isl_ast_expr(definition_expression));
            isl_ast_expr * assignment_expression = isl_ast_expr_add(isl_ast_expr_copy(mem_expression), offset_expr);
            isl_ast_node * assignment = isl_ast_node_alloc_user(assignment_expression);
            // build          definition statement
            
            pth_ast_stmt * stmt = pth_ast_stmt_alloc(pth_assign_stmt);
            stmt->id = isl_id_copy(id);
            stmt->assign.lhs = pth_ast_node_from_isl_ast_node(definition);
            stmt->assign.rhs = pth_ast_node_from_isl_ast_node(assignment);
            
            // add definition statement
            scop = pth_scop_insert_stmt(scop, stmt);
        }
        
        
        
        
        return list;
}




std::string pth_generate_scop_function_declaration(pet_scop * pscop) {

    pscop = pet_scop_align_params(pscop);
    
    pth_scop * scop = pth_scop_alloc(pscop);
    
    
    /* Something fishy here */
    //isl_union_map * dependencies = pth_calculate_dependencies(scop);
            
    //isl_schedule * aschedule  = pth_compute_schedule(scop, dependencies);
//    scop = pth_apply_schedule(scop, aschedule);
 
    // scop = pth_apply_tiling(scop, 32);
    
    /*    isl_ctx * ctx = isl_set_get_ctx(scop->scop->context);
     * isl_printer * printer = isl_printer_to_str(ctx);
    printer = isl_printer_print_union_map(printer, schedule_map);
    std::cout << isl_printer_get_str(printer) << std::endl;
    isl_printer_free(printer);
      */  
    
    scop = pth_scop_populate_array_offsets(scop);
    isl_ast_node_list * definitions_list = pth_scop_populate_array_definitions(scop);
    
    std::stringstream ss;
    ss << "void " << pth_generate_scop_name(pscop) << "\n" << "(" << "\n";

    VarMap arguments = pth_generate_scop_function_prototype_arguments(pscop);

    VarMap::iterator argits = arguments.begin();
    
    while(argits != arguments.end()) {
        ss << argits->second << " " << argits->first;
        argits++;
        if (argits != arguments.end()) ss << "," << "\n";
    }
    ss << "\n";
    ss << ") " << "\n";
    
   // isl_printer * mprinter = pth_get_printer_from_scop(pscop);
    
    isl_union_map * schedule = pet_scop_collect_schedule(pscop);
    isl_union_set * domain = pet_scop_collect_domains(pscop);
    
   // mprinter = isl_printer_print_union_set(mprinter, domain);
    schedule = isl_union_map_intersect_domain(schedule, domain);
    
  
    
    
    
  
   // std::cerr << isl_printer_get_str(mprinter) << "\n";
    
    isl_ast_print_options * options = isl_ast_print_options_alloc(pth_get_ctx_from_scop(pscop));
    isl_ast_build * build = isl_ast_build_from_context(pscop->context);
    options = isl_ast_print_options_set_print_user(options, pth_print_user_statement, scop);
    build = isl_ast_build_set_create_leaf(build, pth_generate_user_statement, scop);
    
     

    
    isl_ast_node * node = isl_ast_build_ast_from_schedule(build, schedule);
    
    
    definitions_list = isl_ast_node_list_add(definitions_list, node);
    
    isl_ast_node * block = pth_ast_node_to_isl_ast_node(pth_ast_node_alloc_block(definitions_list));
    
    isl_printer * printer = pth_get_pretty_printer_from_scop(pscop);
    printer = isl_ast_node_print(block, printer, options);
    ss << isl_printer_get_str(printer) << "\n";
    
    return ss.str();
}


/*isl_multi_pw_aff * somefunc(isl_multi_pw_aff *mpa, isl_id *id, void *user) {
    isl_space * space = isl_multi_pw_aff_get_space(mpa);
    isl_ctx * ctx = isl_space_get_ctx(space);
    isl_printer * printer = isl_printer_to_str(ctx);
    printer = isl_printer_print_id(printer, id);
    printer = isl_printer_print_multi_pw_aff(printer, mpa);
    std::cerr << isl_printer_get_str(printer) << std::endl;
    isl_printer_free(printer);
    
    return mpa;
}*/

/*static isl_multi_pw_aff *pullback_index(
           isl_multi_pw_aff *index, isl_id *id, void *user)
  {
         isl_pw_multi_aff *iterator_map = (isl_pw_multi_aff *) user;
 
          iterator_map = isl_pw_multi_aff_copy(iterator_map);
          return isl_multi_pw_aff_pullback_pw_multi_aff(index, iterator_map);
  }
  */

/*isl_printer * isl_printer_print_call_op(isl_printer * printer, isl_ast_build * build, pet_scop * scop, isl_ast_expr * expr) {
  //  printer = isl_printer_print_str(printer, "call_operation\n");
    
    unsigned n = isl_ast_expr_get_op_n_arg(expr);
  
    for (unsigned i = 0 ; i < n ; i++) {
        isl_ast_expr * op_expr = isl_ast_expr_get_op_arg(expr, i);
        
          isl_ast_expr_type type = isl_ast_expr_get_type(op_expr);
    
          switch(type) {
             case isl_ast_expr_op : { printer = isl_printer_print_op(printer, build, scop, op_expr);} break;
             case isl_ast_expr_id : { 
                isl_id * id  = isl_ast_expr_get_id(op_expr);
               
                if(i == 0) { 
                     pet_stmt * stmt = pth_get_scop_statement(scop, id);
                     
                   
                     
                     
                     
                     if (stmt) { 
                         
                         isl_map * map = isl_map_from_union_map(isl_ast_build_get_schedule(build));
                         map = isl_map_reverse(map);
                         
                         //isl_pw_multi_aff * iterator_map = isl_pw_multi_aff_from_map(map);
                         
                         //isl_id_to_ast_expr * ref_expr = pet_stmt_build_ast_exprs(stmt, build, pullback_index, iterator_map, NULL, NULL);
                         
                         printer = pth_print_scop_body(printer, build, scop, stmt, expr);
                        //printer = pet_stmt_print_body(stmt, printer, ref_expr);
                     }
                } else { 
                  //  printer = isl_printer_print_id(printer, id);
                }
          
          } break;
                case  isl_ast_expr_int : {printer = isl_printer_print_str(printer, "int\n");} break;
                 default : {printer = isl_printer_print_str(printer, "other\n");} 
    }
        
    }
    
    
    return printer;
}
*/
