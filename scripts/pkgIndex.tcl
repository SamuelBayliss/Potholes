
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
    get_platform_library_extension $dir 
    uplevel 1 [list source [file join $dir analysis.tcl] ]    
}} $dir]