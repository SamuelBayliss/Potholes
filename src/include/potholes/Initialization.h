/* 
 * File:   Initialization.h
 * Author: sb1708
 *
 * Created on October 8, 2013, 6:32 PM
 */
#include <tcl.h>
#include <itcl.h>

#ifndef INITIALIZATION_H
#define	INITIALIZATION_H

extern "C" {     
   int Potholes_Init(Tcl_Interp * );
   int Potholes_Unload(Tcl_Interp *);
   int Potholes_SafeInit();
   int Potholes_SafeInit();
   int Potholes_SafeUnload();
}


#endif	/* INITIALIZATION_H */

