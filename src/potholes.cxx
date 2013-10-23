

#include <tcl.h>

#include <itcl.h>

#include <iostream>

#include <map>



#include <isl/ctx.h>


#include <potholes/process.h>
#include <potholes/options.h>
    

#include <potholes/Initialization.h>
#include <potholes/tclBackedObject.h>
#include <potholes/analysis.h>
#include <potholes/project.h>
#include <potholes/scop.h>

using namespace potholes;


int Potholes_Init(Tcl_Interp * interp) {

  if (Itcl_Init(interp) == TCL_ERROR) { 
    return TCL_ERROR;
  }
    
  if (Itcl_RegisterC(interp, "analysis_initialize", Analysis::Create, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }

  if (Itcl_RegisterC(interp, "analysis_destroy", Analysis::Destroy, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }
  
  if (Itcl_RegisterC(interp, "analysis_get", Analysis::Get, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }
 
  if (Itcl_RegisterC(interp, "analysis_transform", Analysis::Transform, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }
  
  
  if (Itcl_RegisterC(interp, "project_initialize", Project::Create, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }
  if (Itcl_RegisterC(interp, "project_files", Project::Files, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }

  
  
  if (Itcl_RegisterC(interp, "project_destroy", Project::Destroy, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }
   if (Itcl_RegisterC(interp, "project_compile", Project::Compile, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }
  
  

  if (Itcl_RegisterC(interp, "scop_filename", Scop::GetFilename, 0, 0) != TCL_OK) { 
    return TCL_ERROR;
  }
  
    return TCL_OK;
}



int Potholes_Unload(Tcl_Interp * interp) { 
     return TCL_OK;
}

int Potholes_SafeInit() {
 return TCL_OK;
}
int Potholes_SafeUnload() { 
 return TCL_OK;
}
