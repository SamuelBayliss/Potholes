
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
#include <piplib/piplibMP.h>

#include <sstream>

void collect_condition_vectors(PipQuast * node, std::vector<PipVector *>& pv) {
  
  if (node->condition) {

  // go left
    collect_condition_vectors(node->next_then, pv);
  // go right
    collect_condition_vectors(node->next_else, pv);
  // add condition
    pv.push_back(node->condition);
  }

}

void collect_list_vectors(PipQuast * node, std::vector<PipList *>& pv) {
  
  if (node->condition) {

  // go left
    collect_list_vectors(node->next_then, pv);
  // go right
    collect_list_vectors(node->next_else, pv);
  // add condition
    // pv.push_back(node->list);
  } 
  else 
    {
      pv.push_back(node->list);
    }
}

/*std::string get_id(unsigned num_parameters) {
  
}
*/

std::string get_param_id(unsigned param_index) {
std::stringstream ss;
 ss << "PR_" << param_index;
  return ss.str();
  
}
std::string get_newparam_id(PipNewparm * np) {
  std::stringstream ss;
  ss << "NP_" << np;
  return ss.str();
}


PipNewparm * get_param_id(unsigned param_index, PipVector * condition, PipQuast * root, bool & found) {
  // grab the newparm associated with the appropriate index. 

  PipNewparm * pnp = NULL;

  if (root->condition != condition) {
    if (!found) pnp = get_param_id(param_index, condition, root->next_then, found);
    if (!found) pnp = get_param_id(param_index, condition, root->next_else, found);
  }

    found = found || (root->condition == condition) ;
      PipNewparm * np = root->newparm;

      while (np != NULL) {
	// look for param_index
	if (np->rank == param_index) {
	  assert(pnp == NULL) ; 
	  pnp = np;
	}

      }
      return pnp;
}

std::vector<std::string> map_condition_inputs (unsigned params, PipVector * condition, PipQuast * root) {

  std::vector<std::string> names;
  // take a condition and get a string of the inputs into the condition
  unsigned i;
  for (i = 0 ; i < params ; i++) { 
    names.push_back(get_param_id(i));
  }

  for (i = params ; i < condition->nb_elements -1 ; i++) {
    bool found = false;
    names.push_back(get_newparam_id(get_param_id(i, condition, root,found)));
  }

  return names;
}

int main()
{
  // loads in a piplib problem and solves it

  FILE * domain_file = fopen("domain2", "r");
  FILE * context_file = fopen("context2", "r");

  assert(domain_file);
  assert(context_file);

  PipMatrix * domain = pip_matrix_read(domain_file);
  PipMatrix * context = pip_matrix_read(context_file);
  PipOptions * opt = pip_options_init();

  PipQuast * pq = pip_solve(domain, context, -1, opt);
  pip_quast_print(stdout, pq, 2);

  // make an array of all condition vectors
  std::vector<PipVector *> pv;
    collect_condition_vectors(pq, pv);
  std::vector<PipList *> lv;
    collect_list_vectors(pq, lv);

  std::cout << "There are " << pv.size() << " Conditions" << std::endl;
  // make an array of all value vectors
  std::cout << "There are " << lv.size() << " Lists" << std::endl;

  // assign all parameters a unique id (root pointer + _num)

  // assign all outputs (treewise) an id (list_pointer + num)

  // assign all new parameters a unique id (pointer)

  // make an input mapping function for all conditions

  std::vector<std::string> inputs;

  std::vector<PipVector *>::iterator pvit;
  for (pvit = pv.begin() ; pvit != pv.end() ; pvit++) { 
    std::cout << *pvit << "\n";
    inputs = map_condition_inputs (context->NbColumns -2, *pvit, pq);
    std::vector<std::string>::iterator input_iterator;
    for (input_iterator = inputs.begin() ; input_iterator != inputs.end() ; input_iterator++) { 
      std::cout << *input_iterator<< "\n";
    }
  }


  // make an input mapping function for all values

  // build verilog graph from all this
 

  pip_close();
  fclose(domain_file);
  fclose(context_file);
  // dumps output
  return 0;
}

/*
  build graph

  // register symbols for all parameters, outputs and newparms with module

  foreach vector
  Take pairwise (1,2)

  VerilogExpression * vt = (int * symbol) + VerilogExpression

  Symbol * (outputSymbol) = vt;

  emit

 */

