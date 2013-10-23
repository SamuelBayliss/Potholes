
#include <isl/ctx.h>

#include <pet.h>

#include <potholes/options.h>
#include <potholes/generate.h>


#include <map>



#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <clang/Rewrite/Core/Rewriter.h>

#include <llvm/Support/CommandLine.h>

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

#include <llvm/Support/Path.h>

#include <iterator>
#include <iostream>
#include <algorithm>
#include <vector>

#include <sstream>
#include <fstream>
using namespace clang::tooling;
using namespace llvm;

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);




    /*template <class T>
    class Label { 
    
    };

    typedef int Integer;
    typedef isl_aff * Affine;

    typedef boost::variant<Label<Integer>, Label<Affine> > EdgeLabel;

    typedef int Node;
    typedef std::pair<Node, Node> Edge;
    typedef std::map<Edge, EdgeLabel> EdgeLabelMap;
     */
    void build_schedule_graph() {

        /*
         
      For each schedule relation, find max dimension size
         
      For each schedule dimension up to dimension size or dimension max of schedule
          
      build graph with labelled edges
         * 
                  
         */
    }

    void build_subtrees() {

    }

 

namespace ct = clang::tooling;

int compile_source_file(isl_ctx * ctx, int argc, const char ** argv) {


    //    struct options *options;

    ct::CommonOptionsParser OptionsParser(argc, argv);

    ct::CompilationDatabase & Database = OptionsParser.getCompilations();
    ct::ClangTool Tool(Database, OptionsParser.getSourcePathList());

    std::vector<std::string> paths = OptionsParser.getSourcePathList();

    potholes::ExtractScop::ScopMap sm;
    potholes::ExtractScop::Locations ld;

    potholes::ExtractScop extractor(ctx, OptionsParser, sm, ld);

    std::for_each(paths.begin(), paths.end(), extractor);

    clang::Rewriter rewriter;

    potholes::ConsumerFactory factory(rewriter, extractor);

    potholes::RewriteCompleteCallback ecb(rewriter, extractor, paths, std::cout);

    int success = Tool.run(newFrontendActionFactory(&factory, &ecb));

    return success;

}

void process_source_file(isl_ctx * ctx, std::string filename, unsigned start, unsigned end) {



    // compile code



    // rewrite code






}

int read_handler(void * data, unsigned char * buffer, size_t size, size_t * size_read) {
    return 0;

}

int process_scop_file(isl_ctx * ctx, std::string filename, unsigned * start, unsigned * end) {

    /*  yaml_parser_t parser;
      yaml_event_t event;
    
      int done = 0;
    
      yaml_parser_initialize(&parser);
    
      char * input = ;
      size_t length = strlen(input);
    
      yaml_parser_set_input_file(&parser, input);
    
      yaml_parser_set_input();
    
      while(!done) { 
          if (!yaml_parser_parse(&parser, &event))
              goto error;
          done = (event.type == YAML_STREAM_END_EVENT);
    
        
          // look for a scalar with tag "start"
          // convert the value of the tag
       
          if (event.type == YAML_SCALAR_EVENT) { 
            
          }
       
        
          yaml_event_delete(&event);
    
      }
    
      yaml_parser_delete(&parser);
      return 1;
    
      error : 
          yaml_parser_delete(&parser);
      return 0;
    
     */
    return 0;
}
