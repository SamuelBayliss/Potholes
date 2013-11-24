/* 
 * File:   project.h
 * Author: sb1708
 *
 * Created on October 21, 2013, 9:50 AM
 */

#ifndef PROJECT_H
#define	PROJECT_H

#include <potholes/tclBackedObject.h>

#include "analysis.h"



namespace potholes {

    class RewriteCallback;
    
class Project : public TclBackedObject<Project> { 
private:
        potholes::RewriteCallback * callback;
	potholes::Analysis * analysis;
        
public:
    Project(int, const char * argv[]);
    ~Project();
    potholes::Analysis * getAnalysis();
    static int Compile(ClientData, Tcl_Interp * , int, const char * argv[]);
    static int Get(ClientData, Tcl_Interp * , int, const char * argv[]);
    void generateCode();
};

};
#endif	/* PROJECT_H */

