/* 
 * File:   Consumer.h
 * Author: sb1708
 *
 * Created on July 3, 2013, 7:09 PM
 */

#ifndef CONSUMER_H
#define	CONSUMER_H

#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <clang/Rewrite/Core/Rewriter.h>

#include <llvm/Support/CommandLine.h>

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

#include <potholes/analysis.h>

#include "rewrite.h"

namespace potholes { 
    
    class Consumer : public clang::ASTConsumer {
        
      //  virtual bool isValidFile(std::string) = 0;
      //  virtual bool isValidScop(clang::SourceRange) = 0;
           virtual void Initialize(clang::ASTContext& Context) = 0;
       virtual bool HandleTopLevelDecl(clang::DeclGroupRef) = 0;

    };
    
    class CompositeConsumer : public clang::ASTConsumer {
    
    private:
        Analysis& analysis;
        RewriteCallback& callback;
    public: 
        CompositeConsumer(potholes::Analysis&, potholes::RewriteCallback&);
        virtual void Initialize(clang::ASTContext& Context);
        virtual bool HandleTopLevelDecl(clang::DeclGroupRef);
        
    };
    
    class ConsumerFactory {
    
    
    private:
//        clang::Rewriter& rewriter;
        potholes::Analysis analysis;
        potholes::RewriteCallback& callback;
        
    public:

        ConsumerFactory(clang::Rewriter& rewriter, potholes::Analysis & analysis, potholes::RewriteCallback& callback);
        clang::ASTConsumer* newASTConsumer();

    };
    
}

#endif	/* CONSUMER_H */

