/* 
 * File:   tclBackedObject.h
 * Author: sb1708
 *
 * Created on October 8, 2013, 6:37 PM
 */


#include <tcl.h>

#include <itcl.h>

#include <iostream>

#include <map>

#include <sstream>

#ifndef TCLBACKEDOBJECT_H
#define	TCLBACKEDOBJECT_H

namespace potholes { 
template <typename T>
class TclBackedObject { 
    protected:
         static std::map<std::string, T *> object_map;
    
        static T * CreateAndRegisterObject(std::string name, int argc, const char * argv[]) {
           
              T * obj = new T(argc, argv);
              
              TclBackedObject::object_map[name] = obj;
            
              return obj;
              
        }
        static void DestroyObject(std::string name) {
            T * obj = TclBackedObject::object_map[name];
            TclBackedObject::object_map.erase(name);
            delete obj;
        }
        
     
          
     
    public:    
        
          static std::string GetObjectName(Tcl_Interp * interp) {
                  const char * val;
                  val = Tcl_GetVar(interp, "this", TCL_LEAVE_ERR_MSG);
          
                  
                  return std::string(val);
            }
        
          static T * GetObjectPtr(Tcl_Interp * interp) { 
              const char * val;
              val = Tcl_GetVar(interp, "this", TCL_LEAVE_ERR_MSG);
              std::string name = val;
              T * obj = TclBackedObject::object_map[name];
              return obj;
          }
          
          static T * Lookup(std::string objectname) {
               std::string lookupname = objectname;
                       
              if (objectname.substr(0,2) != "::") {
                   lookupname = std::string("::") + objectname; 
              }
               
           typename std::map<std::string, T*>::iterator candidate = TclBackedObject::object_map.find(lookupname);
                    if (candidate != TclBackedObject::object_map.end() ) {
                        return candidate->second;
                    } 
           return NULL;
          }
          
         static int Register(Tcl_Interp * interp, T * obj, std::string classname, std::string& objectname) {
              
             char * val;
            
             Tcl_DString buffer;
             Tcl_DStringInit(&buffer);
             Tcl_DStringAppendElement(&buffer, "uplevel");
             Tcl_DStringAppendElement(&buffer, "#0");
             Tcl_DStringAppendElement(&buffer, classname.c_str());
             
             std::stringstream ss;
             ss  << "::" << classname << "::#auto";
             
             Tcl_DStringAppendElement(&buffer, ss.str().c_str());

             val = Tcl_DStringValue(&buffer);
           //  std::cerr << "created " <<  val << "\n";
             
             if (Tcl_Eval(interp, val) != TCL_OK) {
               Tcl_DStringFree(&buffer);
               
//               Tcl_Obj * result = Tcl_GetObjResult(interp);
              // std::cerr << Tcl_GetString(result) << "\n";
               return TCL_ERROR;
             } else {  
            
                          
             Tcl_Obj * result = Tcl_GetObjResult(interp);
             
              objectname = Tcl_GetString(result);
              
             Tcl_IncrRefCount(result);
       //      std::cerr << "Registering " << objectname << "\n";
                TclBackedObject::object_map[objectname] = obj;
                 Tcl_DStringFree(&buffer);
             if (result == NULL ) {
                 std::cerr << "No Result from script" << "\n";
             } else {
        //                  std::cerr << Tcl_GetString(result) << "\n";
             }
             
       

               return TCL_OK;
                }
         }
          
        static int Create(ClientData cData, Tcl_Interp * interp, int argc, const char * argv[]) {
    
        std::string name = GetObjectName(interp);
        CreateAndRegisterObject(name, argc, argv);
         
        return TCL_OK;
        }
        
        static int Destroy(ClientData cData, Tcl_Interp * interp, int argc, const char * argv[]) {
            std::string name = GetObjectName(interp);
        DestroyObject(name);      
        return TCL_OK;
}
        
        
};

template <typename T> std::map<std::string, T *> TclBackedObject<T>::object_map;
}

#endif	/* TCLBACKEDOBJECT_H */

