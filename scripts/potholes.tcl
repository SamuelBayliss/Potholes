#!/usr/bin/tclsh

set auto_path [linsert $auto_path 0 .]
set auto_path [linsert $auto_path 0 [file join $::env(ENVIRONMENT_PLATFORM_DIR) lib tcl]]


package require Potholes

set process_pid [pid]



puts "The process pid = $process_pid"

after 10000

set database "/home/sb1708/Work/shared-tools/PoTHoLeS-AutoESL/demo/compile_commands.json"

set analysis [Potholes::Analysis #auto $database]

foreach source [$analysis get -sources] { 
    puts $source
}

foreach scop [$analysis get -scops] { 
    #puts $scop 
    set filename [$scop filename]
    #puts $filename
    $analysis transform -promote-scop-to-function $scop

}


#    $analysis transform -promote-scop-to-function $scop
    
set solution "ZynqSolution"
set project [Potholes::Project #auto $analysis $solution]

$project compile

foreach file [$project get -files] {
    puts "Transformed Source : $file"
}



itcl::delete object $project
itcl::delete object $analysis

