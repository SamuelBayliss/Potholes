/* 
 * File:   scop.h
 * Author: sb1708
 *
 * Created on October 10, 2013, 7:41 PM
 */

#ifndef SCOP_H
#define	SCOP_H

#include <potholes/tclBackedObject.h>

#include <pet.h>

namespace potholes { 
class Scop : public TclBackedObject<Scop> { 
 public : 
     pet_scop * scop;
     Scop(pet_scop *, std::string name);
     
     static int GetFilename(ClientData, Tcl_Interp * , int, const char * argv[]);
protected : 
    std::string filename;
     
 };
};
#endif	/* SCOP_H */

