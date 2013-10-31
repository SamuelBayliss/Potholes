#include <isl/ctx.h>

#include <potholes/analysis.h>
#include <potholes/extract.h>

#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include "potholes/transform.h"



using namespace clang::tooling;
namespace ct = clang::tooling;

namespace potholes { 

template <> std::map<std::string, potholes::Analysis *> potholes::TclBackedObject<potholes::Analysis>::object_map;

};


potholes::Analysis::~Analysis(){ 
    std::cout << "Deleted Analysis" << std::endl;
}


class isValidFile { 
public:
    bool operator()(std::string file) {

        return false;
    }
    
};


potholes::Analysis::Analysis(int argc, const char * argv[] ) : ctx(isl_ctx_alloc()), 
        sm(potholes::ExtractScop::ScopMap()), ld(potholes::ExtractScop::Locations()), 
        extractor(potholes::ExtractScop(ctx, sm, ld)) { 
    
   compile_commands = argv[1];
     
    std::string ErrorMessage;
    ct::CompilationDatabase * database = ct::CompilationDatabase::autoDetectFromSource(compile_commands, ErrorMessage);
  
    if (database) {    
        std::vector<std::string> files = database->getAllFiles(); 

        sources.insert(sources.begin(), files.begin(),  std::remove_if(files.begin(),  files.end(),  isValidFile() ));
 
        Tool = new clang::tooling::ClangTool(*database, sources); 
     
        extractor.initialize(database);

        std::for_each(sources.begin(), sources.end(), extractor);
        }
    
      std::cout << "Created Analysis" << std::endl;
}

void potholes::Analysis::registerTransform(potholes::Transform * transform) {
     //   std::cout << "Registering code transfomrations" << "\n";
    code_transformations.insert(transform);
}

std::set<potholes::Transform *> & potholes::Analysis::getTransforms() { 
   // std::cout << "Returning code transfomrations" << "\n";
    return code_transformations;
}

int potholes::Analysis::Transform(ClientData cData, Tcl_Interp * interp, int argc, const char * argv[]) {
    
   // std::cout << "Applying Transform" << std::endl;
    
    /* Register a transform and arguments for a particular analysis object */
    potholes::Analysis * analysis = potholes::TclBackedObject<Analysis>::GetObjectPtr(interp);
    potholes::Scop * scop = potholes::TclBackedObject<potholes::Scop>::Lookup(argv[2]);
    
    if (scop) {
        potholes::PromoteScop * transform = new potholes::PromoteScop(*analysis);
        analysis->registerTransform(transform);
    } else { 
        std::cerr << "Can't find Scop" << std::endl;
    }
    
   /* for (unsigned int i = 0 ; i < argc ; i++ ) {
        std::cout << argv[i] << "\n";
        
        }
    */
    return TCL_OK;
}


potholes::Analysis::Files potholes::Analysis::getSources() {
    return sources;
}

int potholes::Analysis::Get(ClientData cData, Tcl_Interp * interp, int argc, const char * argv[]) {

 
    Analysis * ptr = GetObjectPtr(interp);
    
    unsigned n = ptr->extractor.GetScopCount();
    
   
    
    if (ptr) {
//         char * val;
        
         // heap allocated array of Tcl Objects
         Tcl_Obj ** objects = (Tcl_Obj **)(malloc(sizeof(Tcl_Obj *) * n));
                 
    for (unsigned i = 0 ; i < n ; i++ ) {
        std::string name;
        potholes::Scop * scop = ptr->extractor.GetScop(i);
        
        int success = scop->Register(interp, scop, "Potholes::Scop", name);
        if (success == TCL_OK) {
            objects[i] = Tcl_NewStringObj(name.c_str(), -1);
        }
    }
         
          Tcl_Obj * list = Tcl_NewListObj(n, objects);
          Tcl_SetObjResult(interp, list);
         
    }
    
    
    return TCL_OK;
}

 

/*int compile_source_file(isl_ctx * ctx, int argc, const char ** argv) {


    //    struct options *options;

    ct::CommonOptionsParser OptionsParser(argc, argv);

    ct::CompilationDatabase & Database = OptionsParser.getCompilations();
    ct::ClangTool Tool(Database, OptionsParser.getSourcePathList());

    std::vector<std::string> paths = OptionsParser.getSourcePathList();

    potholes::ExtractScop::ScopMap sm;
    potholes::ExtractScop::Locations ld;

    potholes::ExtractScop extractor(ctx, OptionsParser, sm, ld);

    std::for_each(paths.begin(), paths.end(), extractor);

    clang::Rewriter rewriter;

    potholes::ConsumerFactory factory(rewriter, extractor);

    potholes::RewriteCompleteCallback ecb(rewriter, extractor, paths, std::cout);

    int success = Tool.run(newFrontendActionFactory(&factory, &ecb));

    return success;

}*/
