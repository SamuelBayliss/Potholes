/* 
 * File:   affine.h
 * Author: sb1708
 *
 * Created on August 7, 2013, 3:54 PM
 */

#include <potholes/ast.h>

#ifndef AFFINE_H
#define	AFFINE_H

#ifdef	__cplusplus
//extern "C" {
#endif

isl_ast_expr * pth_generate_access_expr(pth_ast_build * build, pth_scop * scop, pth_stmt * stmt, pth_expr * expr );


#ifdef	__cplusplus
//}
#endif

#endif	/* AFFINE_H */

