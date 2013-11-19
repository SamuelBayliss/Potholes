#!/usr/bin/tclsh

set auto_path [linsert $auto_path 0 .]
set auto_path [linsert $auto_path 0 [file join $::env(ENVIRONMENT_PLATFORM_DIR) lib tcl]]


package require Potholes

set database "/home/sb1708/Work/shared-tools/PoTHoLeS-AutoESL/demo/compile_commands.json"

set analysis [Potholes::Analysis #auto $database]

foreach source [$analysis get -sources] { 
    puts $source
}

foreach scop [$analysis get -scops] { 
    puts $scop 
    set filename [$scop filename]
    puts $filename
}


#    $analysis transform -promote-scop-to-function $scop
    
set solution "ZynqSolution"
set project [Potholes::Project #auto $analysis $solution]

#$project compile

#foreach file [$project files -changed] {
#    puts "Transformed Source : $file"
#}



#itcl::delete object $project
itcl::delete object $analysis