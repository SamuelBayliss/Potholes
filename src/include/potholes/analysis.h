#ifndef POTHOLES_ANALYSIS
#define POTHOLES_ANALYSIS

#include <isl/set.h>
#include <isl/polynomial.h>

//typedef isl_basic_set  pth_basic_set ;
//typedef isl_pw_qpolynomial pth_pw_qpolynomial;
//
//struct pth_analysis_script {
//
//  const char * name;
//
//  void emit();
//  void execute();
//
//} ;
//
//pth_analysis_script * pth_analysis_script_alloc(const char *);
//
//typedef enum {PW_QPOLYNOMIAL, BASIC_SET} pth_expression_type; 
//
//typedef enum {CARD} pth_unary_operation_type;
//typedef enum {ADD, SUB} pth_binary_operation_type;
//
//struct pth_expression { 
//  
//  pth_expression_type a;
//
//};
//
///* 
//   Builds up expression tree and saves table of expressions and their targets in script;
//
//   emit() traverses expression tree and writes to file
//   execute() runs script and parses result
//
//*/
//
//struct pth_term_expression : pth_expression { 
//   pth_expression_type datatype;
//   void * data;
//}
//
//struct pth_unary_expression : pth_expression { 
//  pth_unary_operation_type optype;
//  pth_expression * child;
//}
//
//struct pth_binary_expression : pth_expression { 
//  pth_binary_operation_type optype;
//  pth_expression * left;
//  pth_expression * right;
//};
//
//insert_left(pth_binary_expression, pth_expression);
//
//insert_right(pth_binary_expression * , pth_expression);
//
//insert(pth_unary_expression *, pth_expression);
//
//
//// Creates Unary Expression (card)
//pth_unary_expression card(pth_basic_set *);
//
//
//struct pth_script_expression {
//  pth_analysis_script * script;
//  operator pth_pw_qpolynomial * ();
//} ;
//




#endif
