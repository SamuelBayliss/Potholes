
package ifneeded Potholes 1.0 [list apply {dir {
    uplevel 1 [list source [file join $dir preparePackage.tcl] ]
    load [file join $dir ../lib/libPotholes.so] 
    uplevel 1 [list source [file join $dir loadPackage.tcl] ] 

}} $dir]