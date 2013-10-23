/* 
 * File:   rewrite.h
 * Author: sb1708
 *
 * Created on October 17, 2013, 5:23 PM
 */

#ifndef REWRITE_H
#define	REWRITE_H

#include <potholes/analysis.h>


class RewriteCommand {
public:
    typedef enum  {Delete, Insert} Type;
};

namespace potholes { 
    
    class Rewriter { 
    public : 
        virtual void ApplyTransformation(clang::Rewriter&) = 0;
    };
    
   class RewriteCallback : public clang::tooling::EndOfSourceFileCallback {
    private:
        clang::Rewriter & rewriter;
        Analysis & analysis;
        Analysis::Files transformed_files;
//        std::ostream& os;
    public:
        // calls rewrite() on all rewriters registered in analysis
        void run();
        RewriteCallback(clang::Rewriter & rewriter, potholes::Analysis &, std::ostream& os);
        
        void setSourceMgr(clang::SourceManager &, const clang::LangOptions&);
        
        void emitCode(std::string& filename, std::ostream & os);
        
        Analysis::Files getTransformedFiles();
     

    
   // Collates all the changes made and emits them to appropriate files
  
      
  
    
        };
};

#endif	/* REWRITE_H */

