/* 
 * File:   analysis.h
 * Author: sb1708
 *
 * Created on October 8, 2013, 6:47 PM
 */

#ifndef ANALYSIS_H
#define	ANALYSIS_H

#include <potholes/tclBackedObject.h>
#include <potholes/extract.h>
#include <potholes/region.h>

#include <set>



namespace potholes { 

    class Transform;
    
    
class Analysis : public TclBackedObject<Analysis> { 
public:
    typedef std::vector<std::string> Files;
    
private:
    std::string compile_commands;
    isl_ctx * ctx;
    potholes::ExtractScop::ScopMap sm;
    potholes::ExtractScop::Locations ld;
    
   
    std::set<potholes::Region> regions;
    
    std::set<std::string> accelerated_functions;
    std::set<potholes::Transform *> code_transformations;
    Files sources;
    
public:
    // This needs to disappear into some other mechanism eventually
     potholes::ExtractScop extractor;
    
    
    ClangTool * Tool;
~Analysis();
Analysis(int, const char * argv[]);
void registerTransform(potholes::Transform *);
std::set<potholes::Transform *> & getTransforms();
Files getSources(); 

// Get will look up 

static int Get(ClientData, Tcl_Interp * , int, const char * argv[]);
static int Transform(ClientData, Tcl_Interp *, int, const char * argv[]);

};



};


#endif	/* ANALYSIS_H */

