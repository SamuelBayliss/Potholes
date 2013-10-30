#!/usr/bin/tclsh

package provide Potholes 1.0

namespace eval ::Potholes:: { 

    itcl::class Analysis { 

        constructor {CompileCommandsFile} { 
            init $CompileCommandsFile
        }

        destructor {
            destroy
        }

	method init { args } @analysis_initialize
        method destroy { } @analysis_destroy
      #  method get { type } @analysis_get
      #  method transform { args } @analysis_transform
        }

    } 

