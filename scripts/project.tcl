namespace eval ::Potholes:: {  

 itcl::class Project {
    
    constructor { analysis solution } { 
            init $analysis $solution
        }

    destructor { 
      destroy
    }

    method files { } @project_files
    method init { args } @project_initialize
    method destroy { } @project_destroy
    method compile { } @project_compile

}
}