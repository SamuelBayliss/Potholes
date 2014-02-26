/* 
 * File:   select.h
 * Author: sb1708
 *
 * Created on February 12, 2014, 1:54 PM
 */

#ifndef SELECT_H
#define	SELECT_H

#include <potholes/tclBackedObject.h>

namespace potholes { 
    
}


class Interface { 

    
    
};


class Parameters;

class Parameters : public potholes::TclBackedObject<Parameters> {

public:
    Parameters(int, const char * argv[]);
    ~Parameters();

    static int Compile(ClientData, Tcl_Interp * , int, const char * argv[]);
    static int Get(ClientData, Tcl_Interp * , int, const char * argv[]);
   
};


#endif	/* SELECT_H */

