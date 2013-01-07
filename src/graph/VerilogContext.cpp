
/*
  Netlists have domains connected by symbols at the boundaries. 
  All Expressions in a domain belong to the same module
  All Symbols in a domain belong to the same module


 */


#include <graph/ReferenceCountedObject.h>

#include <graph/VerilogContext.h>
#include <graph/VerilogSymbol.h>
#include <graph/VerilogModule.h>
#include <graph/VerilogExpression.h>

#include <sstream>

#include <cassert>
#include <set>
VerilogContext * VerilogContext::pInstance = NULL;

VerilogContext * VerilogContext::getContext()
{
  if (pInstance == NULL) {
    pInstance = new VerilogContext();
  }
  return pInstance;
}

VerilogContext::VerilogContext() {
  Initialize();
}

void VerilogContext::Initialize()
{

}

void VerilogContext::insert( VerilogSymbol &sym ,  VerilogModule * mod , VerilogSymbolDirection::Direction dir)
{
	std::cout << "Added Symbol" << "\n";
	
	// Increment the VerilogSymbol Backing Symbol
	ReferenceCountedObject::increment(sym.getShadow());
	ModuleMapKey key(dir, mod);
	// Register the Symbol in the symbol table
	sy.insert(std::pair<ModuleMapKey,  VerilogSymbol* >(key, dynamic_cast< VerilogSymbol *>(sym.shadow)));

	// Walk through expressions which use Symbol and register them accordingly
}
void VerilogContext::insert(VerilogModule * module)
{
	assert(module);
	ReferenceCountedObject::increment(module);
	
	ml.push_back(module);
}

/*
class 
void print(std::pair<const VerilogContext::ModuleMapKey, VerilogSymbol * > pr) {
  //VerilogContext::ModuleSymbolMapIterator it
  std::cout << pr.second << std::endl;
  }
*/

VerilogSymbol VerilogContext::lookup( std::string& name,  VerilogModule* vm,VerilogSymbolDirection::Direction dir)
{
  // try to find 
  std::cerr << "*** Looking for " << name << "(" << vm << ")" << "***" << std::endl;
  std::pair<ModuleSymbolMapIterator, ModuleSymbolMapIterator> pr = sy.equal_range(ModuleMapKey(dir, vm));
  
  ModuleSymbolMapIterator it = pr.first;
  while(it != pr.second) {
    assert(it->second);
    
    std::cerr << "Check " << name << " " << it->second << " " << it->second->refCount << "\n";
    if (it->second->getName() == name) {
      std::cerr << "Found " << it->second->getName() << " Direction = " << dir << "\n";
      return VerilogSymbol(it->second);
    }
    it++;
  }
  std::cerr << "Didn't Find " << name << dir << "\n";
    return VerilogSymbol(NULL);
}
VerilogModule VerilogContext::lookup(std::string nm)
{
	// looks through the modules
	VerilogModule * module = NULL;
	std::vector<VerilogModule *>::iterator mli;
	for (mli = ml.begin() ; mli != ml.end() ; mli++)
	{
		if ((*mli)->getName() == nm)
		{
			module = *mli;
			break;
		}
	}
	
	return VerilogModule(this,module);
}


void VerilogContext::dump(std::ostream&os )
{
	os << "*** Dump ***" << "\n";
	os << "Symbols : " << "\n";
	ModuleSymbolMapIterator mit;
	for (mit = sy.begin(); mit != sy.end(); mit++) {
	  std::cout << (mit->first).second->getName() << " (" << (mit->first).second << ")" << " (";
		switch ((mit->first).first) {
			case VerilogSymbolDirection::Input :
				std::cout << "(I)";
				break;
			case VerilogSymbolDirection::Output:
				std::cout << "(O)";
				break;
			case VerilogSymbolDirection::Local :
				std::cout << "(L)";
				break;
			default:
				break;
		}
		std::cout << " ";
		std::cout << *(mit->second) << " ";
		std::cout << "\n";

	}
	os << "*** Dump ***" << "\n";
	os << "Expressions : " << "\n";
	
	std::multimap< VerilogExpression *,  VerilogModule*, std::less< VerilogExpression *> >::const_iterator cit;
	for (cit=em.begin(); cit!=em.end(); cit++) {
	  os << cit->first->print() << "\n";
		{
			std::stringstream filename;
			filename << "Expr_" << cit->first;
			cit->first->print_dot_file(filename.str());
		}
	}
	
	
	
}

std::vector<VerilogSymbol> VerilogContext::getSymbols(const VerilogModule * module, unsigned mask) const
{
	// grab all the symbols belonging to a verilog module
	
	std::set<VerilogSymbol, std::less<VerilogSymbol> > ss;
	
	if ((mask & VerilogSymbolDirection::Input) > 0)
	{
		ModuleSymbolMapConstIterator mit ;
		for (mit = sy.begin(); mit != sy.end(); mit++) {
			ModuleMapKey key = mit->first;
			if ((key.first == VerilogSymbolDirection::Input) && (key.second == module)) 
			{
				ss.insert(VerilogSymbol(mit->second));
			
			}
		}
	}
	
	if ((mask & VerilogSymbolDirection::Output) > 0)
	{
		ModuleSymbolMapConstIterator mit ;
		for (mit = sy.begin(); mit != sy.end(); mit++) {
			ModuleMapKey key = mit->first;
			if ((key.first == VerilogSymbolDirection::Output) && (key.second == module)) 
			{
				ss.insert(VerilogSymbol(mit->second));
			}
		}
	}
	
	if ((mask & VerilogSymbolDirection::Local) > 0)
	{
		ModuleSymbolMapConstIterator mit ;
		for (mit = sy.begin(); mit != sy.end(); mit++) {
			ModuleMapKey key = mit->first;
			if ((key.first == VerilogSymbolDirection::Local) && (key.second == module)) 
			{
				ss.insert(VerilogSymbol(mit->second));
			}
		}
	}
	
	
	return std::vector<VerilogSymbol>(ss.begin(), ss.end());
}
VerilogSymbolDirection::Direction VerilogContext::lookup(const VerilogModule * module,VerilogSymbol symbol) const { 
  	ModuleSymbolMapConstIterator mit ;
		for (mit = sy.begin(); mit != sy.end(); mit++) {
			ModuleMapKey key = mit->first;
			if ((key.second == module) && (mit->second == symbol.getShadow())) {
			    return key.first;
			  }
		}
		return VerilogSymbolDirection::None;
}

std::vector<std::string> VerilogContext::getSymbolNames(const VerilogModule * module, unsigned mask) const
{
	// grab all the symbols belonging to a verilog module
	
	std::set<std::string, std::less<std::string> > ss;
	
	if ((mask & VerilogSymbolDirection::Input) > 0)
	{
		ModuleSymbolMapConstIterator mit ;
		for (mit = sy.begin(); mit != sy.end(); mit++) {
			ModuleMapKey key = mit->first;
			if ((key.first == VerilogSymbolDirection::Input) && (key.second == module)) ss.insert(mit->second->getName());
		}
	}
	
	if ((mask & VerilogSymbolDirection::Output) > 0)
	{
		ModuleSymbolMapConstIterator mit ;
		for (mit = sy.begin(); mit != sy.end(); mit++) {
			ModuleMapKey key = mit->first;
			if ((key.first == VerilogSymbolDirection::Output) && (key.second == module)) ss.insert(mit->second->getName());
		}
	}
	
	if ((mask & VerilogSymbolDirection::Local) > 0)
	{
		ModuleSymbolMapConstIterator mit ;
		for (mit = sy.begin(); mit != sy.end(); mit++) {
			ModuleMapKey key = mit->first;
			if ((key.first == VerilogSymbolDirection::Local) && (key.second == module)) ss.insert(mit->second->getName());
		}
	}
	
	
	return std::vector<std::string>(ss.begin(), ss.end());
}

std::vector< VerilogModule *> VerilogContext::getModules() const
{
	std::set< VerilogModule*, std::less< VerilogModule*> > ms;
	
	// go through all the symbols and build a list of modules
	
	ModuleSymbolMapConstIterator mit ;
	for (mit = sy.begin(); mit != sy.end(); mit++) {
		ModuleMapKey key = mit->first;
		ms.insert(key.second);
	}
	ModuleExpressionMapConstIterator met;
	for (met = em.begin(); met != em.end(); met++) {
		ms.insert(met->second);
	}
	
	return std::vector< VerilogModule*>(ms.begin(), ms.end());

}

void VerilogContext::emit(std::ostream& os)
{
	os << "//*******" << "Emit Verilog" << "*******" << "\n";
	// get list of verilog modules
	std::vector< VerilogModule *> modules = getModules();
	std::vector< VerilogModule *>::iterator mit;
	// print out module
	for (mit = modules.begin() ; mit != modules.end() ; mit++)
	{
	  assert(*mit);
		(*mit)->emit(os);
		
	}
}
// No Anti Cycling - Cycling should be prohibited either by checks when a connection is made, or by checks here. 



void recurse_down(VerilogGraphNode * vn, GraphTraversal& gt) {
  if (vn) { 
   
  // get VerilogNode owner
  VerilogGraphActor * owner = vn->owner; 
  assert(owner);
    std::cerr << "Owner is " << owner->print() << std::endl;
  // if owner is a symbol -> we're done
  VerilogSymbol * sym = dynamic_cast<VerilogSymbol *>(owner);
    gt.visit(vn);
  if(sym) return;
  
  // otherwise find the connected output, keep going
  VerilogGraphNode * home = vn;
  while (vn->next != home) {
    if(vn -> direction == VerilogSymbolDirection::Output) {
      // go further down tree
      recurse_down(vn->back, gt);
   
    }
    vn = vn->next;
  }

  }
}
// No Anti Cycling - Cycling should be prohibited 
void recurse_up(VerilogGraphNode * vn, GraphTraversal& gt) {
  if (vn) { 
  // get VerilogNode owner
  VerilogGraphActor * owner = vn->owner;
  assert(owner);
  std::cerr << "Owner is " << owner->print() << std::endl;
  // if owner is a symbol -> we're done
  VerilogSymbol * sym = dynamic_cast<VerilogSymbol *>(owner);
  gt.visit(vn);
  if(sym) return;
  
  // otherwise find the connected output, keep going
  VerilogGraphNode * home = vn;
   if(vn -> direction == VerilogSymbolDirection::Input) {
      // go further down tree
      recurse_up(vn->back, gt);    
    }
   vn = vn->next;

  while (vn != home) {
    if(vn -> direction == VerilogSymbolDirection::Input) {
      // go further down tree
      recurse_up(vn->back, gt);    
    }
    vn = vn->next;
  }

  }
}
std::set<std::pair<VerilogSymbol, VerilogExpression *> > VerilogContext::getExpressionOutputs(const VerilogModule * module) { 
 
  std::set<std::pair<VerilogSymbol, VerilogExpression *> > osyms;

class getOutputSymbols: public GraphTraversal { 
public:
    std::set<std::pair<VerilogSymbol, VerilogExpression *> > actors;
    VerilogExpression * ve;
    void visit(VerilogGraphNode * vt) { 
      // Check Type of node parent
      
      if (vt) { 
	VerilogSymbol * vs = dynamic_cast<VerilogSymbol *>(vt->owner);
	if(vs) { 
	  assert(vt->direction == VerilogSymbolDirection::Input);
	        
	  actors.insert(std::pair<VerilogSymbol, VerilogExpression *>(VerilogSymbol(vs), ve));
	}

	VerilogExpression * ven = dynamic_cast<VerilogExpression *>(vt->owner);
	if (ven) ve = ven;
      }

    };
  };
 // grab all the expressions
 ModuleExpressionMap::iterator mit;
 for (mit = em.begin() ; mit != em.end() ; mit++ )  {
   if (mit->second == module) { 
     // for each symbol, recurse down
     getOutputSymbols gs;
     gs.ve = (*mit).first;
     recurse_down((*mit).first->getList(), gs);
       osyms.insert( gs.actors.begin(), gs.actors.end());
   }
 }
 
 return osyms;

  
}


void VerilogContext::gather_domain_edges(VerilogGraphActor * e, std::vector<VerilogGraphActor *>& inputs, std::vector<VerilogGraphActor *>& outputs) {
  // make a set of inputs

  class printNode: public GraphTraversal { 
public:
    std::vector<VerilogGraphActor*> actors;
    void visit(VerilogGraphNode * vt) { 
      // Check Type of node parent

      if (vt) { 
	VerilogSymbol * vs = dynamic_cast<VerilogSymbol *>(vt->owner);
	if(vs) { 
	  assert(vt->direction == VerilogSymbolDirection::Input);
	        std::cout << "Visit Down : Got to Symbol" << std::endl;
		actors.push_back(vt->owner);
	}
      }
      std::cout << "Visit Down" << std::endl;
    };
  };
class printNodeUp: public GraphTraversal {
public:
 std::vector<VerilogGraphActor*> actors;
    void visit(VerilogGraphNode * vt) { 
      // Check Type of node parent

      if (vt) { 
	VerilogSymbol * vs = dynamic_cast<VerilogSymbol *>(vt->owner);
	if(vs) { 
	  assert(vt->direction == VerilogSymbolDirection::Output);
	        std::cout << "Visit Up : Got to Symbol" << std::endl;
		actors.push_back(vt->owner);
	}

	VerilogConstant * vc = dynamic_cast<VerilogConstant *>(vt->owner);
	if(vc) { 
	  assert(vt->direction == VerilogSymbolDirection::Output);
	        std::cout << "Visit Up : Got to Constant" << std::endl;
		actors.push_back(vt->owner);
	}

      }
      std::cout << "Visit Up" << std::endl;
 


};
  };
  printNodeUp un;
  printNode dn;

  recurse_down(e->getList(), dn);
  std::cerr << "There are " << dn.actors.size() << " actors down" << std::endl;
  recurse_up(e->getList(), un); 
 std::cerr << "There are " << un.actors.size() << " actors up" << std::endl;
  // find output

 inputs.insert(inputs.begin(), un.actors.begin(), un.actors.end());
 outputs.insert(outputs.begin(), dn.actors.begin(), dn.actors.end());
  // take an expression and walk from output to all possible symbols
  // recurse down the tree until you reach an output symbol


  // walk to all possible inputs from the inputs of the symbol

}

void gather_domain_expressions (VerilogGraphActor * e, std::set<VerilogGraphActor *>& ves) { 
  class gatherExpression: public GraphTraversal {
public:
 std::set<VerilogGraphActor *> & exprs;
    gatherExpression(std::set<VerilogGraphActor *>& exprs_) : exprs(exprs_) {};

    void visit(VerilogGraphNode * vt) { 
      // Check Type of node parent

      if (vt)  {
	VerilogExpression * vs = dynamic_cast<VerilogExpression *>(vt->owner);
	if(vs) { exprs.insert(vs); }
      }

    };
  };

gatherExpression ge(ves);

  recurse_down(e->getList(), ge);  
  recurse_down(e->getList(), ge);
}

void gather_domain_symbols( VerilogGraphActor * e,  std::set<VerilogGraphActor *>& vss ) { 
  
}

void VerilogContext::connect(VerilogGraphActor * e,std::vector<VerilogGraphActor *>& actors){
  std::vector<VerilogGraphActor *> inputs;
  std::vector<VerilogGraphActor *> outputs;
  gather_domain_edges(e, inputs, outputs);
  // go through all the actors which are edges of a domain (symbols) and build a set of module owners 
  std::vector<VerilogGraphActor *>::iterator gait;

  std::set<VerilogModule*> owners;

  for(gait = inputs.begin() ; gait != inputs.end() ; gait++) { 
    
    ModuleSymbolMap::iterator mit;
    for (mit = sy.begin() ; mit != sy.end() ; mit++) { 
      if (mit->second == *gait) owners.insert(mit->first.second);
    }

  }
  
  for(gait = outputs.begin() ; gait != outputs.end() ; gait++) { 
     ModuleSymbolMap::iterator mit;
    for (mit = sy.begin() ; mit != sy.end() ; mit++) { 
      if (mit->second == *gait) owners.insert(mit->first.second);
    }
  }
  assert(owners.size() <= 1);
  VerilogModule * module = *(owners.begin());

  if (owners.size() == 1) {
    std::set<VerilogGraphActor *> ves;
    std::set<VerilogGraphActor *> vss;

    gather_domain_expressions(e, ves);
    gather_domain_symbols(e, vss);

    setOwner(ves,  module);
    setOwner(vss,  module);
     

  }


  //gather_domain_expressions(e, expressions);

  // register all the unmatched expressions


  // if there is a single module owner, gather domain nodes 

  // if domain expression is unregistered, register it with appropriate owner. 

  // if domain symbol is unregistered, register it as local

}


 VerilogModule * VerilogContext::find_owner( VerilogSymbol * sym)
{
	// lookup
	ModuleSymbolMapIterator mit ;
	for (mit = sy.begin(); mit != sy.end(); mit++) {
		if (mit->second == sym)
		{
			assert(sym->getRefCount() > 0);
			return (mit->first).second;
		}
	}
	assert(sym->getRefCount() == 0);
	return NULL;

}

 VerilogModule * VerilogContext::find_owner( VerilogExpression * expr)
{
	// lookup in expression table
	return NULL;
}
void  VerilogContext::clear() { 
  // delete all verilog modules
  ml.clear();

  // delete all verilog expressions
  em.clear();
  // delete all verilog symbols
  sy.clear();


}

VerilogModule * VerilogContext::find_unique_owner(std::set<VerilogGraphActor *>& vec)
{
	 VerilogModule * owner = NULL;
	
	std::set<VerilogGraphActor *>::const_iterator git;
	for(git = vec.begin() ; git != vec.end() ; git++)
	{
		assert(*git);
		VerilogSymbol * sym = dynamic_cast<VerilogSymbol*> (*git);
		
		if (sym) 
		{
			if (owner == NULL) 
			{
				owner = find_owner(sym);
			}
			else {
				assert(owner == find_owner(sym));	
			}
		
		}
		VerilogExpression * expr = dynamic_cast<VerilogExpression*> (*git);
		if (expr) 
		{
			if (owner == NULL) 
			{
				owner = find_owner(expr);
			}
			else {
				assert(owner == find_owner(expr));
			}
		}
		
	}
	return owner;
}

void VerilogContext::setOwner(std::set<VerilogGraphActor *>& vec,  VerilogModule * owner)
{
	std::set< VerilogGraphActor *>::const_iterator git;
	for(git = vec.begin() ; git != vec.end() ; git++)
	{
		assert(*git);
		 VerilogSymbol * sym = dynamic_cast< VerilogSymbol*> (*git);
		if (sym && find_owner(sym) == NULL) // if the actor is a symbol
		{
			// insert into symbol table with owner set;
			ModuleMapKey key(sym->getDirection(), owner);
			sy.insert(std::pair<ModuleMapKey, VerilogSymbol* >(key, sym));
		}
		
		 VerilogExpression * expr = dynamic_cast< VerilogExpression*> (*git);
		if (expr && find_owner(expr) == NULL)
		{
			// insert into expression table with owner set;
			em.insert(std::pair< VerilogExpression *,  VerilogModule *>(expr, owner));
			
		}

	}
}




void VerilogContext::dump(std::set<VerilogGraphActor *>& vec, std::ostream& os)
{
	std::set<VerilogGraphActor *>::const_iterator git;
	for(git = vec.begin() ; git != vec.end() ; git++)
	{
		assert(*git);
		VerilogSymbol * sym = dynamic_cast<VerilogSymbol*> (*git);
		
		os << "************"<<"\n";
		
		if (sym) os << "Symbol " << *sym << "\n";
		VerilogExpression * expr = dynamic_cast< VerilogExpression*> (*git);
		if (expr) os << "Expression " << "\n";// << *expr;
		
	}
	 VerilogModule * vm = find_unique_owner(vec);
	std::cout << "Unique Owner is " << vm << "\n";
	setOwner(vec, vm);
	
}
