/* 
 * File:   map.h
 * Author: sb1708
 *
 * Created on December 19, 2012, 12:24 PM
 */

#ifndef MAP_H
#define	MAP_H

#include <isl/map.h>

#include <potholes/set.h>

typedef isl_basic_map pth_basic_map;

// Return an Identity Map
pth_basic_map * pth_identity_basic_map(pth_basic_set *);



#endif	/* MAP_H */

