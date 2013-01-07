/*
 *  VerilogSymbol.cpp
 *  VerilogLibrary
 *
 *  Created by Samuel Bayliss on 28/11/2011.
 *  Copyright 2011 Imperial College. All rights reserved.
 *
 */

/*

  Symbols should have an input and output graphnode attached. 

  both the back pointers should be null. 
  multiple terms can point to the appropriate graphnode (input or output)



 */


#include <graph/VerilogSymbol.h>
#include <graph/VerilogExpression.h>
#include <graph/VerilogValue.h>
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
VerilogGraphActor::VerilogGraphActor() : ReferenceCountedObject(0), shadow(NULL), list(NULL) // private default constructor 
{
	assert(false);
}

VerilogGraphActor::VerilogGraphActor(VerilogGraphActor * shadow) : ReferenceCountedObject(0),shadow(shadow), list(NULL) // private default constructor 
{
	if (shadow == NULL) ReferenceCountedObject::increment(this);
}

void VerilogGraphActor::createGraphNodes(const VerilogGraphNode * node, const VerilogGraphActor * root, Agraph_t * graph) const
{
	
	const VerilogGraphNode * base = node;
	
	std::string base_name;
	
	node->print(std::cerr);
	std::cerr << "\n";
	if (root == node->owner)
	{
		{
			std::stringstream ss ; 
			node->print(ss);
			base_name = ss.str();
			Agnode_t * base_node = agnode(graph, (char *)base_name.c_str());
		}
		
		// gather nodes from back pointer
		if (node -> back)
		{
			// add back pointer
			{
				std::stringstream ss ; 
				node->back->print(ss);
				std::string node_name = ss.str();

				// node pointed to by back
				Agnode_t * new_node = agnode(graph, (char *)node_name.c_str());
				// node with back pointer
				Agnode_t * base_node = agnode(graph, (char *)base_name.c_str());
				Agedge_t * e = agedge(graph, base_node, new_node);
				agsafeset(e, "color", "blue", "");
				//	assert(node->back->back == node);
					e = agedge(graph, new_node, base_node);
				  agsafeset(e, "color", "blue", "");
				
			
			}
			createGraphNodes(node->back, root, graph);
		}
	}
	
	const VerilogGraphNode * current = node;
	
	std::string current_node_name;
	std::string new_node_name;
	
	while( current->next != base) {

		{
			std::stringstream ss;
			current->print(ss);
			current_node_name = ss.str();
		}
		
		current = current->next;
		// add current node
		{
			std::stringstream ss ; 
			
			current->print(ss);
			new_node_name = ss.str();
			Agnode_t * new_node = agnode(graph, (char *)new_node_name.c_str());
			Agnode_t * old_node = agnode(graph, (char *)current_node_name.c_str());
			Agedge_t * e = agedge(graph, old_node, new_node);
			agsafeset(e, "color", "green", "");
		}
		if (current->back)
		{
			// add back pointer
			{
				std::stringstream ss ; 
				current->back->print(ss);
				std::string node_name = ss.str();
				Agnode_t * back_node = agnode(graph, (char *)node_name.c_str());
				Agnode_t * new_node = agnode(graph, (char *)new_node_name.c_str());
				Agedge_t * e = agedge(graph, new_node, back_node);
				agsafeset(e, "color", "blue", "");
				//assert(current->back->back == current);

				if (current->back->back == current) {

				e = agedge(graph, back_node, new_node);
					agsafeset(e, "color", "blue", "");
				}
			}
			assert(current->back->owner != root);
			createGraphNodes(current->back, root,graph);
		}
	}
	if (base->next != base)
	{
		std::stringstream ss;
		base->print(ss);
		current_node_name = ss.str();
		Agnode_t * new_node = agnode(graph, (char *)new_node_name.c_str());
		Agnode_t * old_node = agnode(graph, (char *)current_node_name.c_str());
		Agedge_t * e = agedge(graph, new_node, old_node);
		agsafeset(e, "color", "green", "");
	}
}

void VerilogGraphActor::print_dot_file(std::string filename) const
{

	assert(shadow == NULL); // this must be a shadow actor
	/*		Agraph_t * g;
		GVC_t * gvc;
		
		gvc = gvContext(); 
		
		char * CommandOptions[3];
		char * LayoutEngine= "dot";
		CommandOptions[0] = LayoutEngine;
		char * OutputType =  "-Tdot";
		CommandOptions[1] = OutputType;
		char * OutputFileSpecifier = (char *) calloc(filename.length() + 10, sizeof(char));
		sprintf(OutputFileSpecifier, "-o%s", filename.c_str());
		CommandOptions[2] = OutputFileSpecifier;
		
		gvParseArgs(gvc,3,CommandOptions);
		g = agopen("Statement Graph", AGDIGRAPHSTRICT);
		
		createGraphNodes(this->list, this, g);

		gvLayoutJobs(gvc,g);
		gvRenderJobs(gvc,g);
		gvFreeLayout(gvc,g);
		agclose(g);
		gvFreeContext(gvc);*/
}

VerilogSymbol::VerilogSymbol(std::string nm): VerilogGraphActor(new VerilogSymbol(nm, false)), name(nm), size(16), isReg(false){
	// creates non-shadow object (which is effectively a pointer), doesn't have a list structure inside (list == NULL)
}



void VerilogSymbol::setReg() {
  if (shadow) { 
    VerilogSymbol * sym = dynamic_cast<VerilogSymbol*>(shadow);
    assert(sym);
    sym->setReg();
  } else {
    isReg = true;
  }
}
void VerilogSymbol::setSize(unsigned size) {
  if (shadow) { 
    VerilogSymbol * sym = dynamic_cast<VerilogSymbol*>(shadow);
    assert(sym);
    sym->setSize(size);
  } else {
    this->size = size;
  }
}

VerilogSymbol::VerilogSymbol() : VerilogGraphActor(NULL), name(""), size(16), isReg(false){
	assert(false);
}

VerilogSymbol::VerilogSymbol(std::string nm, bool hasShadow) : VerilogGraphActor(NULL), name(nm), size(16), isReg(false) {
	    assert(!hasShadow);
		std::cout << "Created Shadow Object " << name << " : " << refCount << "\n";

		// should create with no internal nodes, or two internal nodes?
		list = VerilogGraphNode::insert(this, NULL, VerilogSymbolDirection::Input, NULL);
		list = VerilogGraphNode::insert(this, list, VerilogSymbolDirection::Output, NULL);
		// creates a shadow object (has a list structure inside)
}
VerilogSymbol::VerilogSymbol(std::string nm, bool hasShadow, unsigned size) : VerilogGraphActor(NULL), name(nm), size(size), isReg(false) {
	    assert(!hasShadow);
		std::cout << "Created Shadow Object " << name << " : " << refCount << "\n";

		// should create with no internal nodes, or two internal nodes?
		list = VerilogGraphNode::insert(this, NULL, VerilogSymbolDirection::Input, NULL);
		list = VerilogGraphNode::insert(this, list, VerilogSymbolDirection::Output, NULL);
		// creates a shadow object (has a list structure inside)
}
std::string VerilogSymbol::print() const
{
  return "Symbol \"" + getName() + "\"";
}
std::string VerilogSymbol::print_declaration() const
{
  std::stringstream ss;
  ss << " signed ";
  if (getSize() != 1) {
     ss <<  "[" << getSize() -1 << ":" << 0 << "]" << " ";
  }
  ss << getName() ;
  return ss.str();
}
	

bool VerilogSymbol::operator<(const VerilogSymbol &b) const {

  if ((name) < (b.name)) {
    return true;
  } else {
    if (name == b.name) {
      return direction < b.direction;
    }
  }
  return false;

}
bool VerilogSymbol::valid(const VerilogSymbol vs) {
  return (vs.getShadow() != NULL);
}
VerilogSymbol::VerilogSymbol(VerilogSymbol * shadow) : VerilogGraphActor(shadow) , size(16)
{
	if (shadow)
	  {
		name = shadow->getName();
		direction = shadow->getDirection();
		isReg = shadow->isReg;
		size = shadow->size;
		increment(shadow);
	} else {
	  name = "";
	  size = 16;
	  direction = VerilogSymbolDirection::Local;
	  isReg = false;
	}
	
}

VerilogSymbol & VerilogSymbol::operator=(const VerilogSymbol& vs) 
{
  if (this != &vs) {
	vc = vs.vc;
	std::cerr << vs.getName() << "\n";
	if(shadow) {
	  std::cerr << "Operator=:: -refcount is " << refCount << "\n";
	  std::cerr << "Operator=:: -refcount is " << shadow->refCount << "\n";
	  decrement(shadow);
	}
	shadow = vs.getShadow();
	if (shadow) {	
	  std::cerr << "Operator=:: +refcount is " << refCount << "\n";
	  std::cerr << "Operator=:: +refcount is " << shadow->refCount << "\n";
	  increment(shadow);
	}
	name = vs.name;
	direction = vs.direction;
	isReg = vs.isReg;
	size = vs.size;
  }
  return *this;


}


VerilogSymbol::VerilogSymbol(const VerilogSymbol& vs) : VerilogGraphActor(vs.shadow), size(16)
{
	vc = vs.vc;
	if (shadow) {
	  increment(shadow);
	}
	name = vs.name;
	direction = vs.direction;
	isReg = vs.isReg;
	size = vs.size;
	//std::cout << "Copied Symbol " << name << "\n";
}

const std::string VerilogSymbol::getName() const 
{  //std::cerr << "RefCount = " << refCount << "\n";
  if (shadow) {
    // std::cerr << "RefCountShadow = " << shadow->refCount << "\n";
    VerilogSymbol * sh = (VerilogSymbol *)shadow;
    return sh->getName();
  }

	return name;
}

const VerilogSymbolDirection::Direction VerilogSymbol::getDirection() const
{
  if (shadow) {
    VerilogSymbol * vs = dynamic_cast<VerilogSymbol *>(shadow);
    return vs->getDirection();
  }
  return direction;
  //  return vs.lookup(this);
}

VerilogSymbol::~VerilogSymbol()
{
	if (shadow) 
	{
	  std::cout << "Calling Decrement on Shadow" << ((VerilogSymbol *)shadow)->getName() << "\n";
		decrement(shadow);
		//	std::cout << "Called Decrement on Shadow" << "\n";
		//std::cout << "Destroyed Stack Symbol " << name << " \n";
	}
	else {
	  std::cout << "Destroyed Heap Symbol " << name << " \n";
	}

}
VerilogGraphNode * VerilogSymbol::getInput(VerilogGraphActor * actor)
{
	VerilogGraphNode * node = actor->getList();
	while(node->direction != VerilogSymbolDirection::Input)
	{
		node = node->next;
	}
	return node;
}
VerilogGraphNode * VerilogSymbol::getOutput(VerilogGraphActor * actor)
{
  VerilogSymbol * vs = dynamic_cast<VerilogSymbol *>(actor);
  assert(vs);
  return vs->getOutput();
  /*
	VerilogGraphNode * node = actor->getList();
	while(node->direction != VerilogSymbolDirection::Output)
	{
		node = node->next;
	}
	return node;
  */

}

VerilogSymbol VerilogSymbol::operator=(VerilogExpression
				       expr)
{
  //std::cout << "Called Operator VE to VS" << "\n";


  // create a VerilogTermExpression to hold the symbol

  VerilogTermExpression vte(*this) ;
  increment(&vte); // make it permanent
  increment(shadow);
   // VTE input is VerilogExpression output
  //VeriloTermExpression * term_sh = dynamic_cast<VerilogTermExpression *>(vte.getShadow());
  VerilogGraphNode * output = vte.getOutput();

   // VTE output is VerilogSymbol Input
  output->back = VerilogSymbol::getInput(this->getShadow());  
   // VerilogSymbol Input has no back link. 
  // VerilogExpression * expr_sh = dynamic_cast<VerilogExpression *>(expr.getShadow());
 
  // assert(expr_sh);
  // assert(term_sh);
  expr.getOutput()->back = vte.getInput();
  vte.getInput()->back = expr.getOutput();

	// Performs substitution on DFG to add SymbolAdaptor. 

	//	shadow->getList()->back = VerilogExpression::getOutput(expr.getShadow());
	//VerilogExpression::getOutput(expr.getShadow())->back = shadow->getList();
	//list = VerilogGraphNode::insert(shadow, shadow->getList(), VerilogSymbolDirection::Input, VerilogExpression::getOutput(expr.getShadow()));
	return *this;
}

VerilogGraphNode * VerilogSymbol::getOutput() {
  return getInputOutput(VerilogSymbolDirection::Output);
}

VerilogGraphNode * VerilogSymbol::getInput() { 
return getInputOutput(VerilogSymbolDirection::Input);
}

VerilogGraphNode * VerilogSymbol::getInputOutput(VerilogSymbolDirection::Direction direction) {

  // crawl around nodes

  VerilogGraphNode * node = list;

  VerilogGraphNode * output = NULL;

  if(shadow) {
    VerilogSymbol * vs = dynamic_cast<VerilogSymbol *>(shadow);
    if(vs) {
      return vs->getInputOutput(direction);
    } else {
      VerilogConstant * vs = dynamic_cast<VerilogConstant *>(shadow);
      return vs->getOutput();
    }
  }

  if (node && node->direction == direction) return node;
  node = node->next;

  if (node) {
    while (node != list) {
      // look for an output node
    
	if (node->direction == direction) { 
	  output = node;
	}

      node = node->next;
    }
  }

  if (output == NULL) {
    // insert an output node into the list. 
      output = VerilogGraphNode::insert(this, list, direction, NULL);
  }

  return output;


}

std::ostream& operator<<(std::ostream& os , const VerilogSymbol & sym)
{
	os << sym.name;
	return os;
}

/* VerilogSymbol VerilogSymbol::operator=(VerilogTermExpression)
{
	std::cout << "Called Operator VTE to VS" << "\n";
	return *this;
}

VerilogSymbol VerilogSymbol::operator=(VerilogBinaryExpression)
{
	std::cout << "Called Operator VBE to VS" << "\n";
	return *this;
}*/

/*
 private : 

  VerilogContext * vc;
  unsigned size;
  mpz_t value;
  VerilogConstant();
*/

VerilogConstant::VerilogConstant(mpz_t value, bool isShadow) : VerilogGraphActor(NULL) {
  assert(!isShadow);
    mpz_init(this->value);
    mpz_set(this->value, value);
    list = VerilogGraphNode::insert(this, NULL, VerilogSymbolDirection::Output, NULL);

 
  if (mpz_sizeinbase(value, 2) > 15) { 
    size = mpz_sizeinbase(value, 2) + 1;
  } else  {
    size = 16;
  }
}

VerilogConstant::VerilogConstant(mpz_t value) : VerilogGraphActor(new VerilogConstant(value,false)) {
  mpz_init(this->value);
  mpz_set(this->value, value);
  list = NULL;
  if (mpz_sizeinbase(value, 2) > 16) { 
    size = mpz_sizeinbase(value, 2);
  } else  {
    size = 16;
  }

}
VerilogConstant::VerilogConstant(VerilogConstant * ptr) : VerilogGraphActor(ptr){
    vc = ptr->vc;
    size = ptr->size;
    if (shadow) {
      decrement(shadow);
    }
    shadow = ptr;

    if (ptr) {
      assert(ptr->shadow == NULL);
      increment(ptr);
    }

    mpz_init(this->value);
    mpz_set(this->value, ptr->value);

}
VerilogConstant::VerilogConstant() : VerilogGraphActor(NULL) {
	assert(false);
}

VerilogConstant::VerilogConstant(const VerilogConstant& c) : VerilogGraphActor(c.getShadow()) {
  mpz_init(this->value);
  mpz_set(this->value, c.value);
  vc = c.vc;
  size = c.size;

}

VerilogConstant::~VerilogConstant() {
	if (shadow) 
	{
	  //	std::cout << "Calling Decrement on Shadow" << "\n";
		decrement(shadow);
		//	std::cout << "Called Decrement on Shadow" << "\n";
		//std::cout << "Destroyed Stack Symbol " << name << " \n";
	}
	else {
	  //std::cout << "Destroyed Heap Symbol " << name << " \n";
	}
	mpz_clear(value);
}

VerilogGraphNode * VerilogConstant::getOutput(VerilogGraphActor * actor) { 
  
	VerilogGraphNode * node = actor->getList();
	while(node->direction != VerilogSymbolDirection::Output)
	{
		node = node->next;
	}
	return node;
  
}
VerilogGraphNode * VerilogConstant::getOutput() { 
  
  if (shadow) { 
    VerilogConstant * vc = dynamic_cast<VerilogConstant *>(shadow);
    assert(vc);
    return vc->getOutput();
  }
  else {
    VerilogGraphNode * node = list;
    assert(list);
	while(node->direction != VerilogSymbolDirection::Output)
	{
		node = node->next;
	}
	return node;
  }
 
  
}

std::string VerilogConstant::print() const { return "(constant)";};

void VerilogSymbol::emit(std::ostream& os) const { 
  os << getName() ;

};
void VerilogConstant::emit (std::ostream& os) const {
  signed long i = mpz_get_si(value);

  os << SignedToVerilogConstant(size, i);

}
