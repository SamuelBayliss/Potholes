/* 
 * File:   parallel.h
 * Author: sb1708
 *
 * Created on August 29, 2013, 2:46 PM
 */

#ifndef PARALLEL_H
#define	PARALLEL_H

#include <potholes/ast.h>

#include <isl/schedule.h>

isl_union_map * pth_calculate_dependencies(pth_scop * scop);

isl_schedule * pth_compute_schedule(pth_scop *, isl_union_map *);
        
pth_scop * pth_apply_schedule(pth_scop *, isl_schedule * scop);

bool pth_loop_is_parallel(pth_ast_build *, pth_scop *);

#endif	/* PARALLEL_H */

