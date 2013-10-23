/* 
 * File:   potholes.cxx
 * Author: sb1708
 *
 * Created on July 3, 2013, 6:04 PM
 */

#include <cstdlib>
#include <iostream>
using namespace std;

/*
 * 
 */


/* Read in a preprocessed benchmark file and scop file*/

/* Parse the benchmark file and find the part of the AST accessed */

/* Clone that part of the AST*/


//#include <boost/program_options.hpp>
//#include <boost/filesystem.hpp>
#include <fstream>
#include <string>
#include <isl/ctx.h>


#include <potholes/process.h>
#include <potholes/options.h>

//namespace po = boost::program_options;
//namespace fs = boost::filesystem;


int main(int argc, const char** argv) {

    // --scopfile
    // --sourcefile
    
    /* Scop file is processed with pet tools*/
 
    isl_ctx * ctx = isl_ctx_alloc();
    
  /*  po::options_description desc("Allowed options");
    
    const std::string scop_key = "scopfile";
    const std::string source_key = "sourcefile";
    const std::string start_char = "startchar";
    const std::string end_char = "endchar";
    
    desc.add_options()
        ("help", "produce help message")
        (source_key.c_str(), po::value<string>(), "Source file after preprocessing")
        (scop_key.c_str(), po::value<string>(), "SCOP file after preprocessing");
      
        
    
    po::variables_map vm;
    try { 
    po::store(po::command_line_parser(argc,argv).options(desc).run(), vm);
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    po::notify(vm);
    
    std::string scop_filename;
    std::string source_filename;
    
   if(vm.count(scop_key)) {
        scop_filename = vm[scop_key].as<std::string>();
         fs::path p(vm[scop_key].as<std::string>());
                if (!fs::exists(p)) { 
                    std::cout << "File Doesn't Exist (" << p.filename() << ")";
                    return -1;
                }
    } else { 
        std::cout << "Scop File Not Specified" << "\n";
        return -1;
    }
    
    
        if(vm.count(source_key)) {
               source_filename = vm[source_key].as<std::string>();
         fs::path p(source_filename);
                if (!fs::exists(p)) { 
                    std::cout << "File Doesn't Exist (" << p.filename() << ")";
                    return -1;
                }
      
    } else { 
        std::cout << "Source File Not Specified" << "\n";
        return -1;
    }
    
    unsigned start,end;
    
    process_scop_file(ctx, scop_filename, &start, &end);
  */
    
    compile_source_file(ctx, argc, argv);
    
    return 0;
}

