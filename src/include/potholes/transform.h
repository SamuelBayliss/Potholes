/* 
 * File:   transform.h
 * Author: sb1708
 *
 * Created on October 21, 2013, 4:53 PM
 */

#ifndef TRANSFORM_H
#define	TRANSFORM_H

#include <potholes/analysis.h>
#include <potholes/rewrite.h>
#include <potholes/consumer.h>

namespace potholes { 
    
    class Transform : public potholes::Consumer, potholes::Rewriter { 
    public:
        Analysis& analysis;
        // applied to particular scop
        // specific function for consumer initialization
        
        // specific function for rewrite callback (makes changes to rewrite tree in analysis
        
        virtual void Initialize(clang::ASTContext& Context) = 0;
        virtual bool HandleTopLevelDecl(clang::DeclGroupRef d) = 0;
        virtual void ApplyTransformation(clang::Rewriter&) = 0;
        
        Transform(Analysis&);
    };
    
    class PromoteScop : public Transform { 
    
    public:
        PromoteScop(Analysis&);
        virtual void Initialize(clang::ASTContext& Context);
        virtual bool HandleTopLevelDecl(clang::DeclGroupRef d);
        virtual void ApplyTransformation(clang::Rewriter&);
        
    private:
        void removeScop(clang::Rewriter&);
        void insertScop(clang::Rewriter&);
        
    };
    
}

#endif	/* TRANSFORM_H */

