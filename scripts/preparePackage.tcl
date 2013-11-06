rename ::source ::Potholes::Source

set ::Potholes::UnsupportedPlatform -1

proc ::source args {
    if {[lsearch $args "-rsrc"] == -1} { 
	uplevel 1 ::Potholes::Source $args
    } else {
	throw ::Potholes::UnsupportedPlatform 
    }
}