/* 
 * File:   process.h
 * Author: sb1708
 *
 * Created on July 9, 2013, 7:43 PM
 */

#ifndef PROCESS_H
#define	PROCESS_H

#include <isl/ctx.h>

int   process_scop_file(isl_ctx * ctx, std::string filename, unsigned *, unsigned *);
void  process_source_file(isl_ctx * ctx, std::string filename, unsigned, unsigned);
void compile_source_file(isl_ctx * ctx, int, const char **);


#endif	/* PROCESS_H */

