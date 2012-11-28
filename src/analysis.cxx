#include <potholes/analysis.h>

pth_analysis_script * pth_analysis_script_alloc(char const * name ) { 
  pth_analysis_script * script = new pth_analysis_script;
  script->name = strdup(name);
  return script;
}
pth_unary_expression  card(pth_basic_set * bs) { 
  // create a pth_expression and return-by-value
  pth_unary_expression ue; 
  ue.type = PW_QPOLYNOMIAL;
  ue.optype = CARD;
  ue.data = (void *)(bs);
  return ue;
}
