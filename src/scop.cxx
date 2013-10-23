
#include <potholes/scop.h>

potholes::Scop::Scop(pet_scop * scop, std::string name) : scop(scop), filename(name) { 
    
};


int potholes::Scop::GetFilename(ClientData, Tcl_Interp * interp, int, const char * argv[]) {
    potholes::Scop * ptr = GetObjectPtr(interp);
    
    std::string& string = ptr->filename;
    std::cerr << string << "\n";
    // return a filename
    Tcl_Obj * filename_obj = Tcl_NewStringObj(string.c_str(), -1);
    Tcl_SetObjResult(interp, filename_obj);
    return TCL_OK;
    
}