/* 
 * File:   ast.h
 * Author: sb1708
 *
 * Created on August 7, 2013, 7:23 PM
 */

#ifndef AST_H
#define	AST_H

#ifdef	__cplusplus
//extern "C" {
#endif

#include <isl/aff.h>
#include <isl/ast.h>
#include <isl/set.h>
#include <isl/map.h>
#include <isl/vec.h>
#include <isl/list.h>

#include <isl/ast_build.h>

#include <pet.h>

/* An expression is either an integer, an identifier or an operation
 * with zero or more arguments.
 */

struct pth_ast_node;


typedef enum {pth_type_double, pth_type_pointer_to_double} pth_supported_type;




typedef isl_id pth_id;
typedef pet_stmt pth_stmt;
typedef pet_expr pth_expr;

struct pth_ast_expr {
	int ref;

	isl_ctx *ctx;

	enum isl_ast_expr_type type;

	union {
		isl_val *v;
		isl_id *id;
		struct {
			enum isl_ast_op_type op;
			unsigned n_arg;
			isl_ast_expr **args;
		} op;
	} u;
};

typedef struct { 
    pth_ast_expr * first_use; 
    pth_supported_type type;
} pth_id_annotation; 

const char * pth_supported_type_to_str(pth_supported_type type);
pth_id_annotation * pth_id_annotation_set_first_use(pth_id_annotation * annotation, pth_ast_expr * expr);
pth_id_annotation * pth_id_annotation_alloc(pth_supported_type type);

typedef enum   {
        pth_call_stmt,
        pth_assign_stmt
}  pth_stmt_type  ;

struct pth_ast_stmt { 
    /* Map of ids to ast expr */
    isl_id * id;
    pth_stmt_type type;
    
    union { 
        struct { 
       pth_ast_node * lhs;
        pth_ast_node * rhs;
        } assign ;
        pth_ast_node * call;
    } ;
};

pth_ast_stmt * pth_ast_stmt_alloc(pth_stmt_type);

typedef struct pth_array_offset_t {
    pth_id * id;
    pth_id * offset_id;
    struct pth_array_offset_t * next;
} * pth_array_offset_p;

pth_array_offset_p pth_array_offset_alloc();
pth_array_offset_p pth_array_offset_insert(pth_array_offset_p list, pth_id * id, pth_id * offset_id);
pth_id * pth_array_offset_lookup(pth_array_offset_p list, pth_id * id);

struct pth_scop { 
    pet_scop * scop;
    unsigned n_stmt;
    pth_ast_stmt ** stmts;
    pth_array_offset_p array_offsets;
};



pth_scop * pth_scop_alloc(pet_scop *);
typedef pth_scop * pth_scop_p;

pth_scop * pth_scop_insert_stmt(pth_scop *, pth_ast_stmt *);

__isl_give pth_ast_expr *pth_ast_expr_alloc_int_si(isl_ctx *ctx, int i);
__isl_give pth_ast_expr *pth_ast_expr_alloc_op(isl_ctx *ctx,
	enum isl_ast_op_type op, int n_arg);
__isl_give pth_ast_expr *pth_ast_expr_alloc_binary(enum isl_ast_op_type type,
	__isl_take isl_ast_expr *expr1, __isl_take isl_ast_expr *expr2);

//pth_ast_expr * pth_ast_expr_set_child(pth_ast_expr *, );

struct pth_ast_node {
	int ref;

	isl_ctx *ctx;
	enum isl_ast_node_type type;

	union {
		struct {
			isl_ast_node_list *children;
		} b;
		struct {
			isl_ast_expr *guard;
			isl_ast_node *then;
			isl_ast_node *else_node;
		} i;
		struct {
			unsigned degenerate : 1;
			isl_ast_expr *iterator;
			isl_ast_expr *init;
			isl_ast_expr *cond;
			isl_ast_expr *inc;
			isl_ast_node *body;
		} f;
		struct {
			isl_ast_expr *expr;
		} e;
	} u;

	isl_id *annotation;
};

pth_ast_node *pth_ast_node_alloc_block(isl_ast_node_list *);

struct pth_ast_build {
	int ref;

	int outer_pos;
	int depth;

	isl_id_list *iterators;

	isl_set *domain;
	isl_set *generated;
	isl_set *pending;
	isl_multi_aff *values;

	isl_pw_aff *value;

	isl_vec *strides;
	isl_multi_aff *offsets;

	isl_multi_aff *schedule_map;

	isl_union_map *options;

	__isl_give isl_ast_node *(*at_each_domain)(
		__isl_take isl_ast_node *node,
		__isl_keep isl_ast_build *build, void *user);
	void *at_each_domain_user;

	__isl_give isl_id *(*before_each_for)(
		__isl_keep isl_ast_build *context, void *user);
	void *before_each_for_user;
	__isl_give isl_ast_node *(*after_each_for)(
		__isl_take isl_ast_node *node,
		__isl_keep isl_ast_build *context, void *user);
	void *after_each_for_user;

	__isl_give isl_ast_node *(*create_leaf)(
		__isl_take isl_ast_build *build, void *user);
	void *create_leaf_user;

	isl_union_map *executed;
	int single_valued;
};

isl_ctx * pth_ast_build_get_ctx(pth_ast_build * build);

pth_ast_build * pth_ast_build_from_isl_ast_build(isl_ast_build * build);
isl_ast_build * pth_ast_build_to_isl_ast_build(pth_ast_build * build);

isl_id *pth_ast_build_get_iterator_id(pth_ast_build *build, int pos);

isl_ast_expr * pth_ast_expr_to_isl_ast_expr(pth_ast_expr *);
pth_ast_expr * pth_ast_expr_from_isl_ast_expr(isl_ast_expr *);

isl_ast_node * pth_ast_node_to_isl_ast_node(pth_ast_node *);
pth_ast_node * pth_ast_node_from_isl_ast_node(isl_ast_node *);


#ifdef	__cplusplus
//}
#endif

#endif	/* AST_H */

