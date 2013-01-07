/*
 *  Test.cpp
 *  VerilogLibrary
 *
 *  Created by Samuel Bayliss on 28/11/2011.
 *  Copyright 2011 Imperial College. All rights reserved.
 *
 */

#include <iostream>
#include <graph/VerilogContext.h>
#include <graph/VerilogModule.h>
#include <graph/VerilogSymbol.h>
#include <graph/VerilogExpression.h>
int main()
{


	VerilogContext * vc = VerilogContext::getContext();
	// Brackets ensure stack allocated objects are destroyed before the symbol table is dumped
	{
	// This always succeeds in registering a module (or recovering an existing module). 
	
	VerilogModule module(vc, "MyModuleDef");
	
	VerilogSymbol in("Input");
	VerilogSymbol out("Output");
	
	// These can throw exceptions (if an incompatible attempt is made to register symbol with module)
	module << in;
	module >> out;
	
	out = in * in;
		
	}
	std::cout << "Completed" << "\n";
		vc->dump(std::cerr);
	//	vc->emit(std::cerr);
	return 0;
}
