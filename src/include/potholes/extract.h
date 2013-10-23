/* 
 * File:   extract.h
 * Author: sb1708
 *
 * Created on October 9, 2013, 6:34 PM
 */

#include <isl/ctx.h>

#include <pet.h>

#include <potholes/options.h>
#include <potholes/generate.h>
#include <potholes/scop.h>

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

#ifndef EXTRACT_H
#define	EXTRACT_H

namespace potholes {
    class ExtractScop;
}

namespace potholes {

    class ExtractScop {
    private:
        isl_ctx * ctx;
        clang::tooling::CompilationDatabase *  db;
        // multi-map of files to scops

    public:
        typedef std::multimap<std::string, potholes::Scop * > ScopMap;
        typedef std::pair<std::string, potholes::Scop *> ScopPair;
        ScopMap & sm;
       
        class Locations {
        protected:
            std::set<clang::SourceLocation> locations;
        public:
          void addLocation(clang::SourceLocation location);
          std::set<clang::SourceLocation> & getLocations();
          
        };
        
      
         Locations & lm;
         
        isl_ctx * context();
        potholes::Scop * GetScop(std::string filename);
        potholes::Scop * GetScop(unsigned index);
        
        unsigned GetScopCount();
        ExtractScop(isl_ctx * ctx, ScopMap & sm, Locations & lm);
        void initialize(clang::tooling::CompilationDatabase *  db);
        Locations& getLocations();
        void operator() (std::string filename);
        clang::SourceLocation find_insert_location(clang::SourceManager & sm, std::string filename);
       
        
    };
        
};
          
#endif	/* EXTRACT_H */

