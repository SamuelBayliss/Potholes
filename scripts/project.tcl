namespace eval ::Potholes:: {  

 itcl::class Project {
    
     private variable solution;
     private variable analysis;

    constructor { avar svar } {
	set solution $svar   
	set analysis $avar
        init $analysis $solution

	if { [string match "*vivado_hls_bin" [info nameofexecutable]] } {
	    puts "Created Vivado HLS Project"
	    open_project potholes
	    open_solution $solution
	    set_part zynq
	}
    }

    destructor { 
	if { [string match "*vivado_hls_bin" [info nameofexecutable]] } {
	    puts "Closing Vivado HLS Project"
	    close_solution 
	    close_project 
	}


      destroy
    }

    method get { } @project_get
    method init { args } @project_initialize
    method destroy { } @project_destroy
    method compile { } {
	puts "Compiling Project"
	set files [$this get -files ]
	puts $files

	foreach project_file $files { 
	    puts "Project File is $project_file" 
	    set flags [$this get -flags $project_file]
	    
	    foreach flag $flags { 
		puts $flag
	    }

	    add_files -cflags $flags $project_file 
	    set_directive_top -name top [$this get -function $project_file]
	}



    } 

}
}