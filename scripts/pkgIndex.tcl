
package ifneeded Potholes 1.0 [list apply {dir { 
    load [file join $dir ../lib/libPotholes.so] 
    uplevel 1 [list source [file join $dir testPackage.tcl] ] 

}} $dir]