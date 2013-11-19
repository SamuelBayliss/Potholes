package provide Potholes 1.0

namespace eval ::Potholes:: { 

    itcl::class Analysis { 

        constructor {CompileCommandsFile} { 
	    analyse $CompileCommandsFile
        }

        destructor {
          
        }

	method analyse { args } @analysis_initialize

        

    }
}