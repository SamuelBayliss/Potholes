
#include <potholes/analysis.h>
#include <potholes/project.h>
#include <potholes/transform.h>
#include <potholes/consumer.h>
#include <potholes/rewrite.h>
#include <string>

potholes::Analysis * potholes::Project::getAnalysis() { 
  return analysis;
}

    potholes::Project::Project(int val, const char * argv[]) {
        std::cout << "Created Project" << std::endl;
    
        clang::Rewriter rewriter;
   
        analysis = potholes::TclBackedObject<Analysis>::Lookup(argv[1]);

        if (analysis) {
     
            callback = new potholes::RewriteCallback(rewriter, *analysis, std::cout);
            potholes::ConsumerFactory factory(rewriter, *analysis, *callback);
            clang::tooling::ClangTool * Tool = analysis->Tool;
            
            int success = Tool->run(newFrontendActionFactory(&factory, callback));
            
            if (success) {
	       potholes::Analysis::Files files =  callback->getTransformedFiles();
            } else { 
	      // throw exception
	    }
            
        } else { 
            // Need to throw an exception here. 
            std::cout << "Not Found" << "\n";
        }
        
        
   /*   

      

       
        
        
*/
    }
    
    potholes::Project::~Project() { 
              std::cout << "Destroyed Project" << std::endl;
    
    }
    
    void potholes::Project::generateCode() {
        //std::string prefix = "autoesl";
        //callback->emitCode(prefix, std::cout);
       // std::cout << "Generating Transformed Code" << "\n";
    }
    
    int potholes::Project::Get(ClientData data, Tcl_Interp * interp, int argc, const char * argv[]) {


      // assert(argc >= 1);

      std::string argtype = argv[1];

      if (argtype.compare("-files") == 0){ 

          potholes::Project * project = potholes::TclBackedObject<Project>::GetObjectPtr(interp);
	  Analysis * analysis = project->getAnalysis();
        Analysis::Files files = analysis->getTransformedFiles();
          
        Tcl_Obj ** objects = (Tcl_Obj **)(malloc(sizeof(Tcl_Obj *) * files.size()));
        unsigned i = 0;
        for (Analysis::Files::iterator it = files.begin() ; it != files.end() ; it++ ) {
             objects[i++] = Tcl_NewStringObj((*it).c_str(), -1);
        }
          
          Tcl_Obj * list = Tcl_NewListObj(files.size(), objects);
          Tcl_SetObjResult(interp, list); 
	  return TCL_OK;
                   
      } else if (argtype.compare("-flags") == 0) {
	
	std::string filename = argv[2];
	
	potholes::Project * project = potholes::TclBackedObject<Project>::GetObjectPtr(interp);
	Analysis * analysis = project->getAnalysis();

	assert(analysis);

	std::vector<std::string> flags;
	analysis->getFlags(filename, flags);

	Tcl_Obj ** objects = (Tcl_Obj **)(malloc(sizeof(Tcl_Obj *) * flags.size()));
	unsigned i = 0;
	for (std::vector<std::string>::iterator it = flags.begin() ; it != flags.end() ; it++ ) {
             objects[i++] = Tcl_NewStringObj((*it).c_str(), -1);
        }
	Tcl_Obj * list = Tcl_NewListObj(flags.size(), objects);
	Tcl_SetObjResult(interp, list);
	  return TCL_OK;
      } else if (argtype.compare("-function")) { 

	std::string filename = argv[2];

	// get the correct analysis object   
	potholes::Project * project = potholes::TclBackedObject<Project>::GetObjectPtr(interp);
	Analysis * analysis = project->getAnalysis();
	potholes::FunctionName name = analysis->getAcceleratedFunction(filename);

	Tcl_Obj * object = Tcl_NewStringObj(name.c_str(), -1);
	Tcl_SetObjResult(interp, object);
	
	  return TCL_OK;
      }


          return TCL_ERROR;
          
    }
    
     int potholes::Project::Compile(ClientData data, Tcl_Interp * interp, int argc, const char * argv[]) {
         std::cout << "Compiling Code" << "\n";
         
         // lookup "this" pointer
         
         potholes::Project * project = potholes::TclBackedObject<Project>::GetObjectPtr(interp);
         
         if (project) { 
             project->generateCode();
         } else {
               std::cout << "Project not found" << "\n";
               return TCL_ERROR;
             
         }
         
         return TCL_OK;
     }
