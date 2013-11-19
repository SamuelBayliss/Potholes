namespace eval ::Potholes:: {  

 itcl::class Project {
    
    constructor { analysis solution } { 
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
	    close_solution $solution
	    close_project potholes
	}


      destroy
    }

    method files { } @project_files
    method init { args } @project_initialize
    method destroy { } @project_destroy
    method compile { } @project_compile

}
}