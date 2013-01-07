#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN 

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>


#include <graph/VerilogContext.h>
#include <graph/VerilogModule.h>
#include <graph/VerilogSymbol.h>
#include <graph/VerilogExpression.h>



BOOST_AUTO_TEST_CASE( my_test )

{	VerilogContext * vc = VerilogContext::getContext();
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
	//	vc->dump(std::cerr);
	//	vc->emit(std::cerr);

  BOOST_CHECK( true) ;
}

/*
  Tests to check scoping 
 */

/* Check Sum of reference count
   
 */

/*
  Test to check inputs and outputs added to module
 */
/*
  Test to check graph when constant assignment done
 */

/*
  Test to check multiplication of an input (squaring) to give correct graph
 */

/*
  Test to check reduction gives correct graph
 */

/* 
   Test to check header has correct items in generated code
 */

/*
  External Test checks code can be parsed. 
 */
