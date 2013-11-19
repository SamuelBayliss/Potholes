
proc get_platform_library_extension { dir } { 
    switch $::tcl_platform(os) { 
    Darwin {
        load [file join $dir ../lib/libPotholes.dylib] 
    }
    Linux { 
         load [file join $dir ../lib/libPotholes.so] 
    }
    default {
    error "Error : Unsupported Platform"
    }
    }
    

}

package ifneeded Potholes 1.0 [list apply {dir {
#    set ::env(ITCL_LIBRARY) [file join $::env(ENVIRONMENT_PLATFORM_DIR) lib tcl]   
    uplevel 1 [list source [file join $dir preparePackage.tcl] ]
    get_platform_library_extension $dir 
    #uplevel 1 [list source [file join $dir loadPackage.tcl] ] 
    uplevel 1 [list source [file join $dir analysis.tcl] ]
    uplevel 1 [list source [file join $dir scop.tcl] ]
    uplevel 1 [list source [file join $dir project.tcl] ]  
}} $dir]