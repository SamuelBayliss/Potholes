/* 
 * File:   set.h
 * Author: sb1708
 *
 * Created on December 19, 2012, 12:24 PM
 */

#ifndef SET_H
#define	SET_H

#include <isl/set.h>

typedef isl_basic_set pth_basic_set;

isl_basic_set * pth_basic_set_to_isl_basic_set(pth_basic_set * bset);


#endif	/* SET_H */

