#!/usr/bin/tclsh

set auto_path [linsert $auto_path 0 .]

package require Potholes

set database "/home/sb1708/Work/shared-tools/PoTHoLeS-AutoESL/demo/compile_commands.json"

set analysis [Potholes::Analysis #auto $database]

foreach scop [$analysis get -scops] { 
    set filename [$scop filename]
    $analysis transform -promote-scop-to-function $scop
    
}

set solution "VivadoSolution"

set project [Potholes::Project #auto $analysis $solution]

$project compile

foreach file [$project files -changed] {
    puts "Transformed Source : $file"
}



itcl::delete object $project
itcl::delete object $analysis