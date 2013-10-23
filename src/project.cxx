
#include <potholes/analysis.h>
#include <potholes/project.h>
#include <potholes/transform.h>
#include <potholes/consumer.h>
#include <potholes/rewrite.h>

    potholes::Project::Project(int val, const char * argv[]) {
        std::cout << "Created Project" << std::endl;
    
        clang::Rewriter rewriter;
   
        //     std::cout << "Analysis is " << argv[1] << "\n";
 
        potholes::Analysis * analysis = potholes::TclBackedObject<Analysis>::Lookup(argv[1]);

        if (analysis) {
     
            callback = new potholes::RewriteCallback(rewriter, *analysis, std::cout);
            potholes::ConsumerFactory factory(rewriter, *analysis, *callback);
            clang::tooling::ClangTool * Tool = analysis->Tool;
            
            int success = Tool->run(newFrontendActionFactory(&factory, callback));
            
            if (success) {
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
    
    int potholes::Project::Files(ClientData data, Tcl_Interp * interp, int argc, const char * argv[]) {
          potholes::Project * project = potholes::TclBackedObject<Project>::GetObjectPtr(interp);
        
          Analysis::Files files = project->callback->getTransformedFiles();
          
        Tcl_Obj ** objects = (Tcl_Obj **)(malloc(sizeof(Tcl_Obj *) * files.size()));
        unsigned i = 0;
        for (Analysis::Files::iterator it = files.begin() ; it != files.end() ; it++ ) {
             objects[i++] = Tcl_NewStringObj((*it).c_str(), -1);
        }
          
          Tcl_Obj * list = Tcl_NewListObj(files.size(), objects);
          Tcl_SetObjResult(interp, list);
         
          
          return TCL_OK;
          
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