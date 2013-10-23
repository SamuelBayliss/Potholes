
package ifneeded Potholes 1.0 [list apply {dir { 
    load [file join $dir ../lib/libPotholes.dylib] 
    uplevel 1 [list source [file join $dir analysis.tcl] ] 
    uplevel 1 [list source [file join $dir scop.tcl] ] 
    uplevel 1 [list source [file join $dir project.tcl] ] 
}} $dir]