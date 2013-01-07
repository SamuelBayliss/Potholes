/*
 *  VerilogModule.cpp
 *  VerilogLibrary
 *
 *  Created by Samuel Bayliss on 28/11/2011.
 *  Copyright 2011 Imperial College. All rights reserved.
 *
 */

#include <graph/VerilogModule.h>
#include <graph/VerilogContext.h>
#include <graph/VerilogExpression.h>
#include <graph/VerilogPrintingFunctions.h>
#include <iostream>
#include <sstream>
VerilogModule::VerilogModule(VerilogContext * vc, std::string nm) : VerilogGraphActor(new VerilogModule(vc, nm, false)), vc(vc), name(nm){

	// looks for module reference with the same name in the Verilog Context;
	
	VerilogModule module = vc->lookup(nm);
	
	if (module.shadow)
	{
		delete shadow; // deletes the old shadow
		shadow = module.shadow;
		increment(shadow);
	}
	else {
		vc->insert(dynamic_cast<VerilogModule*>(shadow)); // i.e. register the shadow register
	}

	
}

VerilogModule::VerilogModule(VerilogContext * vc, VerilogModule * vm) : VerilogGraphActor(vm), vc(vc)
{
	if(vm) {
		name = vm->name;
	}
}
VerilogSymbol VerilogModule::getSymbol(std::string symbolname) {
  
  VerilogSymbol candidate = vc->lookup(symbolname, static_cast<VerilogModule *>(this->getShadow()), VerilogSymbolDirection::Local);  
  std::cerr << "looked for local " << symbolname << "\n";
  if (VerilogSymbol::valid(candidate)) return candidate;
  candidate = vc->lookup(symbolname, static_cast<VerilogModule *>(this->getShadow()), VerilogSymbolDirection::Output);  
  std::cerr << "looked for output " << symbolname << "\n";
  if(VerilogSymbol::valid(candidate)) return candidate; 
      candidate = vc->lookup(symbolname, static_cast<VerilogModule *>(this->getShadow()), VerilogSymbolDirection::Input);
      std::cerr << "looked for input " << symbolname << "\n";
  return candidate;

}

VerilogModule::VerilogModule(VerilogContext * vc, std::string nm, bool hasShadow) : VerilogGraphActor(NULL), vc(vc), name(nm){
	
	assert(!hasShadow);
	
	// looks for module reference with the same name in the Verilog Context;
	
}

std::string VerilogModule::getName() const {
	if (shadow)
	{
		return dynamic_cast<VerilogModule*>(shadow)->getName();
	}
	else {
		return name;
	}

};

void VerilogModule::operator|(VerilogSymbol& vs)
{
  vc->insert(vs, static_cast<VerilogModule *>(this->getShadow()), VerilogSymbolDirection::Local);
}

void VerilogModule::operator<<(VerilogSymbol& vs)
{
  vc->insert(vs, static_cast<VerilogModule *>(this->getShadow()), VerilogSymbolDirection::Input);
}

void VerilogModule::operator>>(VerilogSymbol& vs)
{
  vc->insert(vs, static_cast<VerilogModule *>(this->getShadow()), VerilogSymbolDirection::Output);
}

void VerilogModule::emit_header(std::ostream& os) const
{
	os << "module " << name;
	
	os << Separator(COMMA) << vc->getSymbolNames(this, VerilogSymbolDirection::Input | VerilogSymbolDirection::Output) << ";" << "\n";

}

std::vector<std::string>transform(VerilogContext* vc, const VerilogModule * module,const std::vector<VerilogSymbol>& symbols){
  std::vector<VerilogSymbol>::const_iterator nit;
  std::vector<std::string> decl;
  std::cerr << symbols.size() << "symbols present in " << module->getName() << "\n";
  for(nit = symbols.begin() ; nit != symbols.end() ; nit++) { 
    // get the type of symbol   
    std::stringstream ss;

    VerilogSymbolDirection::Direction dir = vc->lookup(module, *nit);    std::cerr<< "Decl" << (*nit).getName() << dir  <<"\n";
    switch (dir) {
    case VerilogSymbolDirection::Output : ss << "output "; break;
    case VerilogSymbolDirection::Input : ss << "input " ; break;
    case VerilogSymbolDirection::Local : {
      if ((*nit).isRegister()) { 
	ss << "reg ";
      } else {
	ss << "wire ";
      }
      
    } ; break;
    default : assert(false);
    }

    ss << nit->print_declaration();
    decl.push_back(ss.str());
  }
  return decl;
}

void VerilogModule::emit_symbols(std::ostream& os) const
{	os << Separator(SEMICOLON) << Separator(NEWLINE) <<  Enclosure(NONE) << transform(vc, this, vc->getSymbols(this, VerilogSymbolDirection::Input | VerilogSymbolDirection::Output | VerilogSymbolDirection::Local)) << ";" << "\n";
}

void VerilogModule::emit_footer(std::ostream& os) const
{
  os << "endmodule" << "\n";
}


void VerilogModule::emit_expressions(std::ostream& os) const
{
  std::set<std::pair<VerilogSymbol, VerilogExpression *> > ExpressionOutputs; 
  std::set<std::pair<VerilogSymbol, VerilogExpression *> >::iterator ops;
  ExpressionOutputs = vc->getExpressionOutputs(this);
  os << "// There are " << ExpressionOutputs.size() << " Expression Outputs" << "\n";
  for (ops = ExpressionOutputs.begin() ; ops != ExpressionOutputs.end() ; ops++) { 
    VerilogExpression::emit(os, (*ops).first,(*ops).second );
  }
  
}

void VerilogModule::emit_fragments(std::ostream& os) const
{
  std::vector<std::string>::const_iterator it;
  for (it = fragments.begin() ; it != fragments.end() ; it++)  {
    os << *it;
  }
}


void VerilogModule::emit(std::ostream& os ) const
{
	emit_header(os);
	emit_symbols(os);
	emit_expressions(os);

	emit_fragments(os);
	emit_footer(os);
}
