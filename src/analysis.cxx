#include <isl/ctx.h>

#include <potholes/analysis.h>
#include <potholes/extract.h>

#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include "potholes/transform.h"

#include <sstream>
#include <algorithm>

using namespace clang::tooling;
namespace ct = clang::tooling;

namespace potholes { 

template <> std::map<std::string, potholes::Analysis *> potholes::TclBackedObject<potholes::Analysis>::object_map;

};


potholes::Analysis::~Analysis(){ 
  //  std::cout << "Deleted Analysis" << std::endl;
}


class isValidFile { 
public:
    bool operator()(std::string file) {

        return false;
    }
    
};
void potholes::Analysis::addAcceleratedFunction(potholes::FileName file_name, potholes::FunctionName function_name) {

  accelerated_functions.insert(potholes::FunctionPair(file_name, function_name));

}

void potholes::Analysis::addTransformedFile(potholes::FileName source, potholes::FileName destination)
{
  transformed_files.insert(potholes::TransformedPair(source, destination));
}

potholes::FileName potholes::Analysis::getSourceFile(potholes::FileName destination) { 

  TransformedMap::iterator tmit;

  for (tmit = transformed_files.begin() ; tmit != transformed_files.end() ; tmit++) { 
    if (tmit->second == destination) { 
      return tmit->first;
    }
  }
  return "";
}

potholes::FileName potholes::Analysis::getTransformedFile(potholes::FileName source) { 
  TransformedMap::iterator tmit = transformed_files.find(source);

  return tmit->second;

}

// in future - have this function throw an exception if there are no accelerated functions

std::string potholes::Analysis::getAcceleratedFunction(potholes::FileName name) { 

  Analysis::FunctionMap::iterator fit = accelerated_functions.find(name);
  if (fit != accelerated_functions.end() ) { 
    return fit->second;
  }

  return "";

}

potholes::Analysis::Analysis(int argc, const char * argv[] ) : ctx(isl_ctx_alloc()), 
							       sm(potholes::ExtractScop::ScopMap()), ld(potholes::ExtractScop::Locations()), database(NULL),
							       extractor(potholes::ExtractScop(ctx, sm, ld)) { 
    
    compile_commands = argv[1];
  // std::cerr << argc << "command"  << "\n"; 
     std::string ErrorMessage;
     database = ct::CompilationDatabase::autoDetectFromSource(compile_commands, ErrorMessage);
  
     if (database) {    
        std::vector<std::string> files = database->getAllFiles(); 

	      sources.insert(sources.begin(), files.begin(),  std::remove_if(files.begin(),  files.end(),  isValidFile() ));
 
	     Tool = new clang::tooling::ClangTool(*database, sources); 
     
         extractor.initialize(database);

         std::for_each(sources.begin(), sources.end(), extractor);
     }
    
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

potholes::Analysis::Files potholes::Analysis::getTransformedFiles() { 
  potholes::Analysis::Files files;

  TransformedMap::iterator tmit;
  for (tmit= transformed_files.begin() ; tmit!= transformed_files.end() ; tmit++) { 
    files.push_back(tmit->second);

  }
  return files;
}

potholes::Analysis::Files potholes::Analysis::getSources() {
    return sources;
}

void potholes::Analysis::getFlags(potholes::FileName filename, std::vector<std::string>& flags) { 

  potholes::FileName source = getSourceFile(filename);

  std::vector<ct::CompileCommand> commands  = database->getCompileCommands(source);
  
  // filter out include and define keywords crudely


   std::cout << source << " " <<  commands.size() << std::endl;
   //  assert(commands.size() == 1);  

   std::vector<ct::CompileCommand>::iterator it;
  for (it = commands.begin() ; it != commands.end() ; it++) { 
    
    // std::string& command = it->CommandLine;

    std::vector<std::string> tokens = it->CommandLine;

    //    istringstream iss(command);
    //  std::copy(istream_iterator<std::string>(iss),istream_iterator<std::string>(), back_inserter<std::vector<std::string> >(tokens));

    std::vector<std::string>::iterator token_iterator;
   
    for (token_iterator = tokens.begin() ; token_iterator != tokens.end() ; token_iterator++) { 
  
     
         std::cerr << "Hello" << *token_iterator<< "\n";
	 /*
    if (token_iterator->find("-I") != std::string::npos) {
	flags.push_back(*token_iterator);
	std::cout << *token_iterator << "\n";
      }

      if (token_iterator->find("-D") != std::string::npos) {
	flags.push_back(*token_iterator);
	std::cout << *token_iterator << "\n";
      }

    }
    std::cout << "Hello World"<< "\n";
*/

    }
    std::cerr << "And Again World"<< "\n";
  }
   
  }

int potholes::Analysis::getSources(Tcl_Interp * interp) {
  Analysis * ptr = GetObjectPtr(interp);
  
  if (ptr) { 
   const potholes::Analysis::Files & files = ptr->getSources();
   std::cerr << "There are " << files.size() << " sources" << "\n";
   Tcl_Obj ** objects = (Tcl_Obj **)(malloc(sizeof(Tcl_Obj *) * files.size()));
    for (unsigned i = 0 ; i < files.size() ; i++ ) { 
      objects[i] = Tcl_NewStringObj(files[0].c_str(), -1);
    }
    Tcl_Obj * list = Tcl_NewListObj(files.size(), objects);
    Tcl_SetObjResult(interp, list);
  }
    return TCL_OK;
}
int potholes::Analysis::getScops(Tcl_Interp * interp) {
    Analysis * ptr = GetObjectPtr(interp);
    
     if (ptr) {  
         unsigned n = ptr->extractor.GetScopCount();
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


int potholes::Analysis::Get(ClientData cData, Tcl_Interp * interp, int argc, const char * argv[]) {

  if (argc == 2) { 

    if (std::string(argv[1]) == "-scops"){ 
      /* Sets Interpreter Result */
      return getScops(interp);
    } else if (std::string(argv[1]) == "-sources") {
      /* Sets Interpreter Result */
     return getSources(interp);
    } else {
      std::cerr << "unknown type" << std::endl;
    }
  }    
  return TCL_ERROR;
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
