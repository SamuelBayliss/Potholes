/* 
 * File:   ast.cxx
 * Author: sb1708
 *
 * Created on August 7, 2013, 7:59 PM
 */

#include <potholes/ast.h>
#include <isl/id.h>
pth_ast_build * pth_ast_build_from_isl_ast_build(isl_ast_build * build) { 
    return (pth_ast_build *)(build);
}

isl_ast_expr * pth_ast_expr_to_isl_ast_expr(pth_ast_expr * expr) {
     return (isl_ast_expr *)(expr);
}

pth_ast_expr * pth_ast_expr_from_isl_ast_expr(isl_ast_expr * expr) {
     return (pth_ast_expr *)(expr);
}

pth_ast_node * pth_ast_node_from_isl_ast_node(isl_ast_node * node) { 
    return (pth_ast_node *)(node);
}

isl_ast_node * pth_ast_node_to_isl_ast_node(pth_ast_node * node) { 
     return (isl_ast_node *)(node);
}

pth_id_annotation * pth_id_annotation_alloc(pth_supported_type type) { 
    pth_id_annotation * annotation = (pth_id_annotation *)(malloc(sizeof(pth_id_annotation)));
    annotation->first_use = NULL;
    annotation->type = type;
    return annotation;
}

pth_id_annotation * pth_id_annotation_set_first_use(pth_id_annotation * annotation, pth_ast_expr * expr) {
    annotation->first_use = expr;
    return annotation;
}

const char * pth_supported_type_to_str(pth_supported_type type) { 
    switch (type) {
        case pth_type_double : {
            return "double";
        } break;
        case pth_type_pointer_to_double : { 
            return "double *";
        }
    }
    return "";
}

pth_ast_expr *pth_ast_expr_alloc_int_si(isl_ctx *ctx, int i) {
    
    	pth_ast_expr *expr;

	expr = isl_calloc_type(ctx, pth_ast_expr);
	if (!expr)
		return NULL;

	expr->ctx = ctx;
	isl_ctx_ref(ctx);
	expr->ref = 1;
	expr->type = isl_ast_expr_int;
	expr->u.v = isl_val_int_from_si(ctx, i);
	if (!expr->u.v)
		return (pth_ast_expr *)(isl_ast_expr_free(pth_ast_expr_to_isl_ast_expr(expr)));

	return expr;
    
}

isl_ctx * pth_ast_build_get_ctx(pth_ast_build * build) { 
   return isl_set_get_ctx(build->domain);
}

pth_ast_expr *pth_ast_expr_alloc_op(isl_ctx *ctx, enum isl_ast_op_type op, int n_arg) {

    
    /* Create a new operation expression of operation type "op",
 * with "n_arg" as yet unspecified arguments.
 */


	pth_ast_expr *expr;

	expr = isl_calloc_type(ctx, pth_ast_expr);
	if (!expr)
		return NULL;

	expr->ctx = ctx;
	isl_ctx_ref(ctx);
	expr->ref = 1;
	expr->type = isl_ast_expr_op;
	expr->u.op.op = op;
	expr->u.op.n_arg = n_arg;
	expr->u.op.args = isl_calloc_array(ctx, isl_ast_expr *, n_arg);

	if (n_arg && !expr->u.op.args)
		return (pth_ast_expr *)(isl_ast_expr_free(pth_ast_expr_to_isl_ast_expr(expr)));

	return expr;

}
pth_ast_expr *pth_ast_expr_alloc_binary(enum isl_ast_op_type type, isl_ast_expr *expr1, isl_ast_expr *expr2) {
    /* Create an expression representing the binary operation "type"
 * applied to "expr1" and "expr2".
 */

	isl_ctx *ctx;
	pth_ast_expr *expr = NULL;

	if (!expr1 || !expr2)
		goto error;

	ctx = isl_ast_expr_get_ctx(expr1);
	expr = pth_ast_expr_alloc_op(ctx, type, 2);
	if (!expr)
		goto error;

	expr->u.op.args[0] = expr1;
	expr->u.op.args[1] = expr2;

	return expr;
error:
	isl_ast_expr_free(expr1);
	isl_ast_expr_free(expr2);
	return NULL;

}

 pth_ast_node *pth_ast_node_alloc(isl_ctx *ctx,
          enum isl_ast_node_type type)
  {
          pth_ast_node *node;
  
          node = isl_calloc_type(ctx, pth_ast_node);
          if (!node)
                  return NULL;
  
          node->ctx = ctx;
          isl_ctx_ref(ctx);
          node->ref = 1;
          node->type = type;
  
          return node;
  }

/* Create a block node with the given children.
 691  */
pth_ast_node *pth_ast_node_alloc_block(
         isl_ast_node_list *list)
 {
          pth_ast_node *node;
          isl_ctx *ctx;
 
        if (!list)
                 return NULL;
 
         ctx = isl_ast_node_list_get_ctx(list);
         node = pth_ast_node_alloc(ctx, isl_ast_node_block);
         if (!node)
                 goto error;

        node->u.b.children = list;
 
        return node;
 error:
        isl_ast_node_list_free(list);
        return NULL;
}

isl_id *pth_ast_build_get_iterator_id(pth_ast_build *build, int pos) { 
  
	if (!build)
		return NULL;

	return isl_id_list_get_id(build->iterators, pos);
}

pth_array_offset_p pth_array_offset_alloc() {
    return NULL;
}
pth_array_offset_p pth_array_offset_insert(pth_array_offset_p list, pth_id * id, pth_id * offset_id) {
   pth_array_offset_p nlist =  (pth_array_offset_p)malloc(sizeof(pth_array_offset_t));
   nlist->id = id;
   nlist->offset_id = offset_id;
   nlist->next = list;
   return nlist;
}
pth_id * pth_array_offset_lookup(pth_array_offset_p list, pth_id * id) {
    if (list == NULL) return NULL;
    if (id == list->id) return list->offset_id;
    return pth_array_offset_lookup(list->next, id);
} 

pth_scop * pth_scop_alloc(pet_scop * pscop) { 
    pth_scop * scop = (pth_scop *)(malloc(sizeof(pet_scop)));
    scop->n_stmt = 0;
    scop->scop = pscop;
    scop->stmts = (pth_ast_stmt **)(malloc(sizeof(pth_ast_stmt *)));
    scop->array_offsets = pth_array_offset_alloc();
    return scop;
}
pth_scop * pth_scop_insert_stmt(pth_scop* scop, pth_ast_stmt* stmt) {
    scop->n_stmt +=1;
    scop->stmts = (pth_ast_stmt **)(realloc(scop->stmts, sizeof(pth_stmt *) * scop->n_stmt));
    scop->stmts[scop->n_stmt -1] = stmt;
    return scop;
    
}