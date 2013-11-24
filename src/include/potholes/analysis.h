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


namespace ct = clang::tooling;

namespace potholes { 

    class Transform;
    typedef std::string FileName;
    typedef std::string FunctionName;
    typedef std::pair<potholes::FileName, potholes::FunctionName> FunctionPair;
    typedef std::pair<potholes::FileName, potholes::FileName> TransformedPair;
class Analysis : public TclBackedObject<Analysis> { 
public:


    typedef std::vector<potholes::FileName> Files;
    typedef std::map<potholes::FileName, potholes::FunctionName> FunctionMap;
    typedef std::map<potholes::FileName, potholes::FileName> TransformedMap;
private:
    std::string compile_commands;
    isl_ctx * ctx;
    potholes::ExtractScop::ScopMap sm;
    potholes::ExtractScop::Locations ld;
    ct::CompilationDatabase * database;
   
    std::set<potholes::Region> regions;
    
    FunctionMap accelerated_functions;
    TransformedMap transformed_files;
    std::set<potholes::Transform *> code_transformations;
    Files sources;
protected:
    
public: 
 Files getSources();
    // This needs to disappear into some other mechanism eventually
     potholes::ExtractScop extractor;
    
    
    ClangTool * Tool;
~Analysis();
Analysis(int, const char * argv[]);
void registerTransform(potholes::Transform *);
std::set<potholes::Transform *> & getTransforms();
 

void addAcceleratedFunction(potholes::FileName, potholes::FunctionName);
 void addTransformedFile(potholes::FileName, potholes::FileName);

 FileName getSourceFile(potholes::FileName);
 FileName getTransformedFile(potholes::FileName);
 
 Files getTransformedFiles();

std::string getAcceleratedFunction(potholes::FileName);
static int getScops(Tcl_Interp * interp);
static int getSources(Tcl_Interp * interp);
void getFlags(potholes::FileName, std::vector<std::string>&);

// Get will look up 

static int Get(ClientData, Tcl_Interp * , int, const char * argv[]);
static int Transform(ClientData, Tcl_Interp *, int, const char * argv[]);

};



};


#endif	/* ANALYSIS_H */

