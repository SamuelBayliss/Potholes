/*
 *  VerilogExpression.cpp
 *  VerilogLibrary
 *
 *  Created by Samuel Bayliss on 29/11/2011.
 *  Copyright 2011 Imperial College. All rights reserved.
 *
 */

#include <graph/VerilogExpression.h>
#include <cassert>

#include <graph/DebugStream.h>

/*VerilogExpression::VerilogExpression() : VerilogGraphActor(new 
{
	list = NULL;
}
*/
VerilogGraphNode * VerilogGraphNode::insert(VerilogGraphActor * owner, VerilogGraphNode * node, VerilogSymbolDirection::Direction dir,  VerilogGraphNode * edge)
{
	VerilogGraphNode * nn = new VerilogGraphNode();
	dout << "Created Graph Node" <<
	  //VerilogSymbol * vs = dynamic_cast<VerilogSymbol *>(owner);
	  //if (vs) dout << vs.getName();
	  dout << owner->print() << 	  "\n";

	nn->back = edge;
	if (edge) {
	  // Only Expressions should have edges doubly-linked. Symbols should be singly linked 
	  //(i.e. term links to output node of symbol) 
	  VerilogExpression * expr = dynamic_cast<VerilogExpression* >(edge->owner);
	  if (expr) edge->back = nn;
	}
	nn->direction = dir;
	nn->owner = owner;
	if (node == NULL)
	{
		nn->next = nn;
		return nn;
	}
	
	nn->next = node->next;
	node->next = nn;
	return nn;	
}

void VerilogGraphNode::print(std::ostream& os) const
{
  assert(owner);

	os << owner->print() << " : ";
	switch (direction) {
		case VerilogSymbolDirection::Input:
			os << "Input";
			break;
		case VerilogSymbolDirection::Output:
			os << "Output";
			break;
		default:
			assert(false);
			break;
	}
	os << " : " << this;
}

VerilogExpression::VerilogExpression() : VerilogGraphActor(NULL) // should never be called (private constructor)
{	
	assert(false);
}

VerilogExpression::VerilogExpression(VerilogGraphActor * shadow) : VerilogGraphActor(shadow) // should create shadow expression
{
	
}

std::string VerilogExpression::print() const
{
	return "An Expression";
}
VerilogGraphNode * VerilogExpression::getOutput(VerilogGraphActor * actor)
{
  assert(actor);
  VerilogGraphNode * node = actor->getList();
  if (actor->getShadow()) {
    assert(actor->getList() == NULL);
    return getOutput(actor->getShadow());
  } else {
    assert(actor->getList());
	while(node->direction != VerilogSymbolDirection::Output)
	{
	  if (node->next == actor->getList()) {
	    assert(false);
	  }
		node = node->next;
	}
  }
	return node;
}

VerilogGraphNode * VerilogTermExpression::getInput(const VerilogGraphActor * actor)
{ 
  assert(actor);
  VerilogGraphNode * node = actor->getList();
  if (actor->getShadow()) {
    assert(actor->getList() == NULL);
    return getInput(actor->getShadow());
  } else {
    assert(actor->getList());
	while(node->direction != VerilogSymbolDirection::Input)
	{
	  if (node->next == actor->getList()) {
	    assert(false);
	  }
		node = node->next;
	}
  }
  return node;
}


void gather(VerilogGraphNode * node, VerilogGraphActor * root, std::vector<VerilogGraphActor *>& nodes)
{

	const VerilogGraphNode * base = node;
	nodes.push_back(node->owner);
	if (root == node->owner)
	{
		// gather nodes from back pointer
		if (node -> back)
		{
			gather(node->back, root, nodes);
		}
	}
	
	const VerilogGraphNode * current = node;
	
	while( current->next != base) {
		current = current->next;
		if (current->back)
		{
			
			gather(current->back, root,nodes);
		}
	}
}

/* This gather method gathers all the nodes up to the nearest symbol */

/*
  If Symbol Input is connected to Expressions in this domain : ( signal is an output from this domain)
  If Symbol Output is connected to Expressions in this domain : (signal is an input into this domain)
 */


/* This gather method gathers all the nodes which point to other domains */

/* First Node in Sorted Order is a domain key */

/* Domain Keys only valid until the graph is altered */

/* This function builds a unique set of GraphNodes to span all neighbouring domains*/



/* This functions finds the unique module to which a domain is registed (either one or zero registered modules) */

/* This function registers all the operators in this domain with an owner module) */

/* Algorithm keeps Key Set (To Visit)
   Algorith keeps Key Set (Visited)

   Put Adjacent Nodes in Set "To Visit"
   Visit one, put adjacent nodes in "To Visit"
   Put domain in visited 
   Keep pulling out of To Visit and populating adjacent nodes until all domains are visited

*/

/*
  If Module:Input is in Symbol Map
  Domain Propagaing from Symbol Output is also in Module:Input

  If Module:Output is in Symbol Map, Domain propagaing from symbol input is also in module

  If domain boundaries are known, propagate forwards? => No for now, never propagate forwards but give warning if a expression subtree will be deleted on going out of scope. 


 */


/*
void gather(VerilogGraphNode * node, std::vector<VerilogGraphActor *>& nodes)
{

	const VerilogGraphNode * base = node;
	nodes.push_back(node->owner);
	if (root == node->owner)
	{
		// gather nodes from back pointer
		if (node -> back)
		{
			gather(node->back, root, nodes);
		}
	}
	
	const VerilogGraphNode * current = node;
	
	while( current->next != base) {
		current = current->next;
		if (current->back)
		{
			
			gather(current->back, root,nodes);
		}
	}
}
*/

VerilogTermExpression::VerilogTermExpression(VerilogSymbol & sym, bool hasShadow) : VerilogExpression(NULL)
{
	assert(!hasShadow);
	// invoked to create shadow term (which in turn is NOT shadowed)
	
	// Add Input Symbol to Expression Loop

	// Assumption that symbols are inputs is wrong. 
	assert(sym.getOutput());
	//	list= VerilogGraphNode::insert(this,this->list,VerilogSymbolDirection::Input, sym.getShadow()->getList());
	list= VerilogGraphNode::insert(this,NULL,VerilogSymbolDirection::Input, sym.getOutput());
	// find Output node of symbol and use that as link edge

	// Add Output Symbol to Expression Loop
	 assert(sym.getInput());
	 list = VerilogGraphNode::insert(this, this->list, VerilogSymbolDirection::Output, sym.getInput());
	std::vector< VerilogGraphActor * > actors;
	
	gather(this->list, this, actors); // gather collects together a domain
	std::cerr << "Connecting TermExpression" << std::endl;
	VerilogContext::getContext()->connect(this, actors);
	
	
}

void VerilogExpression::emit(std::ostream& os, VerilogSymbol vs, VerilogExpression * ve) {
  os << "//" << vs.print() << "\n";

  // find the output node feeding vs input
  
  VerilogGraphNode * vi = vs.getInput();
  


  assert(ve);
  
  os << "assign " << vs.getName() << " = ";
  VerilogTermExpression * vte = dynamic_cast<VerilogTermExpression*>(ve);
  if (vte) vte->emit(os);
  VerilogBinaryExpression * vbe = dynamic_cast<VerilogBinaryExpression*>(ve);
  if (vbe) vbe->emit(os);
  VerilogReduction * vr = dynamic_cast<VerilogReduction*>(ve);
  if (vr) vr->emit(os);
  VerilogMux * vm = dynamic_cast<VerilogMux*>(ve);
  if (vm) vm->emit(os);


  // call emit on it's owner
  os << ";\n";

}

// emit on a symbol should just emit symbol name

// emit on VerilogTermExpression output node should just call emit on symbol

// emit on BinaryTermExpression output node should just call emit on inputs and sandwich in a suitable separator

// emit on Reduction output node should just call emit on all inputs and sandwich in a suitable separator
void VerilogTermExpression::emit (std::ostream& os) const {
  
  os << "(";
  VerilogGraphNode * vgn = getInput();
  assert(vgn->back);
  VerilogGraphActor * input = vgn->back->owner;
  assert(input);
  
    VerilogTermExpression * vte = dynamic_cast<VerilogTermExpression*>(input);
  if (vte) vte->emit(os);
  VerilogBinaryExpression * vbe = dynamic_cast<VerilogBinaryExpression*>(input);
  if (vbe) vbe->emit(os);
  VerilogReduction * vr = dynamic_cast<VerilogReduction*>(input);
  if (vr) vr->emit(os);
    VerilogSymbol * vs = dynamic_cast<VerilogSymbol*>(input);
  if (vs) vs->emit(os);

    VerilogConstant * cs = dynamic_cast<VerilogConstant*>(input);
  if (cs) cs->emit(os);
  os << ")";

}

VerilogTermExpression::VerilogTermExpression(VerilogConstant & sym, bool hasShadow) : VerilogExpression(NULL)
{
	assert(!hasShadow);
	// invoked to create shadow term (which in turn is NOT shadowed)
	
	// Add Input Symbol to Expression Loop

	// Assumption that symbols are inputs is wrong. 
	assert(sym.getOutput());
	//	list= VerilogGraphNode::insert(this,this->list,VerilogSymbolDirection::Input, sym.getShadow()->getList());
	list= VerilogGraphNode::insert(this,NULL,VerilogSymbolDirection::Input, sym.getOutput());
	// find Output node of symbol and use that as link edge
	list= VerilogGraphNode::insert(this,this->list,VerilogSymbolDirection::Output, NULL);
	std::vector< VerilogGraphActor * > actors;
	
	//	gather(this->list, this, actors);
	//	VerilogContext::getContext()->dump(actors, std::cerr);
	std::cerr << "Connecting TermExpression Constant" << std::endl;
		VerilogContext::getContext()->connect(this, actors);
	
}
VerilogTermExpression::VerilogTermExpression(VerilogSymbol&  sym) : VerilogExpression(new VerilogTermExpression(sym, false))
{
	// creates primary term (with shadow backing automatically generated)
	
  dout << "Called Constructor VS (" << sym.getName() << ") to VTE" << "\n";
	assert(list == NULL);
	ReferenceCountedObject::increment(sym.getShadow());
	
}

VerilogTermExpression::VerilogTermExpression(VerilogConstant& sym) : VerilogExpression(new VerilogTermExpression(sym, false))
{
	// creates primary term (with shadow backing automatically generated)
	
  dout << "Called Constructor VS ( constant ) to VTE" << "\n";
	assert(list == NULL);
	ReferenceCountedObject::increment(sym.getShadow());
	
}

void VerilogBinaryExpression::emit (std::ostream& os) const {
  os << "(";  

 
  VerilogGraphNode * vge = list;
  if (vge->direction == VerilogSymbolDirection::Input) { 
      VerilogTermExpression * vte = dynamic_cast<VerilogTermExpression*>(vge->back->owner);
  if (vte) vte->emit(os);
  VerilogBinaryExpression * vbe = dynamic_cast<VerilogBinaryExpression*>(vge->back->owner);
  if (vbe) vbe->emit(os);
  VerilogReduction * vr = dynamic_cast<VerilogReduction*>(vge->back->owner);
  if (vr) vr->emit(os);
    VerilogSymbol * vs = dynamic_cast<VerilogSymbol*>(vge->back->owner);
  }
  //if (vge->next != list) os << "*";

  vge = vge->next;
  while(vge != list) { 
  if (vge->direction == VerilogSymbolDirection::Input) { 
    VerilogTermExpression * vte = dynamic_cast<VerilogTermExpression*>(vge->back->owner);
  if (vte) vte->emit(os);
  VerilogBinaryExpression * vbe = dynamic_cast<VerilogBinaryExpression*>(vge->back->owner);
  if (vbe) vbe->emit(os);
  VerilogReduction * vr = dynamic_cast<VerilogReduction*>(vge->back->owner);
  if (vr) vr->emit(os);
    VerilogSymbol * vs = dynamic_cast<VerilogSymbol*>(vge->back->owner);
  if (vr) vs->emit(os); 

  if (vge->next != list) os << "*";
  }
  vge = vge->next;
  }
  
  os << ")";


}

VerilogBinaryExpression::VerilogBinaryExpression(VerilogExpression a, VerilogExpression b) : VerilogExpression(new VerilogBinaryExpression(a,b,false))
{
  dout << "Called Constructor VE, VE to VBE" << "\n";
	
	// Two input expressions and output expression in loop
	
	//VerilogGraphNode::insert(list, VerilogSymbolDirection::Input, &a);
	//VerilogGraphNode::insert(list, VerilogSymbolDirection::Input, &b);
	//VerilogGraphNode::insert(list, VerilogSymbolDirection::Output, NULL);
}
VerilogBinaryExpression::VerilogBinaryExpression(VerilogExpression a, VerilogExpression b, bool hasShadow) : VerilogExpression(NULL)
{
	
	assert(!hasShadow);
	
	dout << "Called Shadow Constructor VE, VE to VBE" << "\n";
	
	assert(a.getShadow()); // check you've been passed a pointer type rather than a shadow type
	assert(b.getShadow()); // check you've been passed the pointer object rather than the shadow type
	
	// Two input expressions and output expression in loop
	list= VerilogGraphNode::insert(this,this->list,VerilogSymbolDirection::Input, a.getOutput());
	list= VerilogGraphNode::insert(this,this->list,VerilogSymbolDirection::Input, b.getOutput());
	list = VerilogGraphNode::insert(this, this->list, VerilogSymbolDirection::Output, NULL);
	std::vector< VerilogGraphActor * > actors;
	gather(this->list, this, actors);

	/* Find Actors without owners */
	std::cerr << "Connecting TermExpression Binary" << std::endl;
	VerilogContext::getContext()->connect(this, actors);
	//	VerilogContext::getContext()->dump(actors, std::cerr);
}


VerilogExpression   operator*(VerilogExpression  a, VerilogExpression   b)
{
  dout << "Called operator VE * VE" << "\n";
	
	return a;
}

VerilogExpression  operator*(VerilogTermExpression a, VerilogTermExpression  b)
{
  dout << "Called operator VTE * VTE" << "\n";
	VerilogBinaryExpression vbe(a, b);
	return vbe;
}



VerilogReduction::VerilogReduction(VerilogReduction::Type type, std::vector<VerilogExpression> expressions, bool hasShadow) : VerilogExpression(NULL)
{
	list = VerilogGraphNode::insert(this, NULL, VerilogSymbolDirection::Output, NULL);

	std::vector<VerilogExpression>::iterator veit;
	std::cout << "THERE ARE " << expressions.size() << " ESXPRESSIONS" << std::endl;
	for (veit = expressions.begin() ; veit != expressions.end() ; veit++)
	  {
	    list = VerilogGraphNode::insert(this,list,VerilogSymbolDirection::Input, (veit)->getOutput());
	  }

      

}


VerilogReduction::VerilogReduction(VerilogReduction::Type type, std::vector<VerilogExpression> expressions) : VerilogExpression(new VerilogReduction(type, expressions, false))
{

}

void VerilogReduction::emit(std::ostream& os) const{
  // os << "(red)";  

  VerilogGraphNode * vge = list;
  if (vge->direction == VerilogSymbolDirection::Input) { 
      VerilogTermExpression * vte = dynamic_cast<VerilogTermExpression*>(vge->back->owner);
  if (vte) vte->emit(os);
  VerilogBinaryExpression * vbe = dynamic_cast<VerilogBinaryExpression*>(vge->back->owner);
  if (vbe) vbe->emit(os);
  VerilogReduction * vr = dynamic_cast<VerilogReduction*>(vge->back->owner);
  if (vr) vr->emit(os);
    VerilogSymbol * vs = dynamic_cast<VerilogSymbol*>(vge->back->owner);
  } 
  vge = vge->next;
  if (vge->next != list) os << "+";


  while(vge != list) { 
  if (vge->direction == VerilogSymbolDirection::Input) { 
    VerilogTermExpression * vte = dynamic_cast<VerilogTermExpression*>(vge->back->owner);
  if (vte) vte->emit(os);
  VerilogBinaryExpression * vbe = dynamic_cast<VerilogBinaryExpression*>(vge->back->owner);
  if (vbe) vbe->emit(os);
  VerilogReduction * vr = dynamic_cast<VerilogReduction*>(vge->back->owner);
  if (vr) vr->emit(os);
    VerilogSymbol * vs = dynamic_cast<VerilogSymbol*>(vge->back->owner);
  if (vr) vs->emit(os); 


  if (vge->next != list) os << "+";
  }
  vge = vge->next;
  }
  
  // os << ")";


}

VerilogMux::VerilogMux(VerilogTermExpression & term)  : VerilogExpression(new VerilogMux(term, false)){
  std::cout << "Created Mux" << std::endl;
}


VerilogMux::VerilogMux(VerilogTermExpression & term, bool hasShadow) : VerilogExpression(NULL) {
	assert(!hasShadow);
	
	dout << "Called Shadow Constructor VTE->VerilogMux" << "\n";
	
	// Two input expressions and output expression in loop

	list = VerilogGraphNode::insert(this, NULL, VerilogSymbolDirection::Input, term.getOutput());

	std::vector< VerilogGraphActor * > actors;
	gather(this->list, this, actors);

	/* Find Actors without owners */
	std::cerr << "Connecting VerilogMux Binary" << std::endl;
	VerilogContext::getContext()->connect(this, actors);

}
void VerilogMux::operator>>(VerilogTermExpression& vte) { 

  list = VerilogGraphNode::insert(this, list, VerilogSymbolDirection::Output, vte.getInput());
	std::vector< VerilogGraphActor * > actors;
	gather(this->list, this, actors);
	
	/* Find Actors without owners */
	std::cout << "Connecting VerilogMux Binary with " << actors.size() << std::endl;
	VerilogContext::getContext()->connect(this, actors);

}
void VerilogMux::emit(std::ostream& os) const { 
  os << "VerilogMux" << "\n";
}

