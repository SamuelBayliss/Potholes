#include <potholes/analysis.h>

#include <iostream>
#include <string>

#include <isl/set.h>

#include <string>

#include <potholes/potholes.h>

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char ** argp) { 

    po::options_description desc("Allowed options");
    
    desc.add_options()
            ("help", "produce help message")
          ;
    
    po::positional_options_description p;
    p.add("input-file", -1);
    
    
    po::options_description hidden("Hidden Options");
    hidden.add_options()
           ("input-file", po::value<std::vector<std::string> >(), "input file")
        ;
           
    
    po::options_description all("All Options");
    all.add(desc).add(hidden);
           
    po::variables_map vm;
    po::store(po::command_line_parser(argc,argp).options(all).positional(p).run(), vm);
    
    po::notify(vm);
    
    if(vm.count("help")) {
        std::cout << desc << "\n";    
        return 1;
    }
    
    if(vm.count("input-file")) {
        std::vector<std::string> files = vm["input-file"].as<std::vector<std::string> >();
        for (std::vector<std::string>::iterator it = files.begin(); it != files.end() ; it++)
        {
                fs::path p(*it);
                if (!fs::exists(p)) { 
                    std::cout << "Input File Doesn't Exist (" << *it << ")";
                    return -1;
                }
                
        }
        
   
    } else { 
        std::cout << "No Input File Specified" << std::endl;
        return -1;
    }
    
//  
//  std::string ss = "{ [x] : x > 0 and x < 100 }";
//  std::cout << ss.c_str() << "\n";
//
//  isl_ctx * ctx = isl_ctx_alloc();
//
//
//  isl_basic_set * bs = isl_basic_set_read_from_str(ctx, ss.c_str());
//  isl_printer * printer = isl_printer_to_str(ctx);
//  printer = isl_printer_print_basic_set(printer, bs);
//  std::cout << "basic_set is " << isl_printer_get_str(printer) << "\n"; 
//
//  isl_printer_free(printer);
//
//  pth_analysis_script * script = pth_analysis_script_alloc("script");
// 
//
//
//  isl_basic_set_free(bs);
//  isl_ctx_free(ctx);
   
    pth_context * ctx = pth_context_alloc();
    
     std::vector<std::string> files = vm["input-file"].as<std::vector<std::string> >();
        for (std::vector<std::string>::iterator it = files.begin(); it != files.end() ; it++)
        {
                  
        std::cerr << "Parsing ("<< *it <<  ")" << std::endl;
    
    pth_scop * scop = pth_parse_file(ctx, (*it));
     
    if (scop)  {
        
        // This will tell me statements which lie within scop
        
        // find those statements in the AST, crawl upwards until for loop bounds are met and prune tree. 
        
        
        
        pth_scop_dump(scop);
        
        std::cout << "Parsing ("<< *it <<  ")" << " Completed" << std::endl; 
        
        
    } else { 
        std::cout << "Parsing ("<< *it <<  ")" << " Failed" << std::endl;
    }
                
    
    
    
        }
    
  std::cout << "Done" << "\n";

  return 0;
}
