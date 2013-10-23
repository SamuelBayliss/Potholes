/* 
 * File:   Visitor..h
 * Author: sb1708
 *
 * Created on July 3, 2013, 7:03 PM
 */


#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>


#ifndef VISITOR__H
#define	VISITOR__H

namespace Potholes { 
    class Visitor : public clang::RecursiveASTVisitor<Potholes::Visitor>{ 
        
    };
}

#endif	/* VISITOR__H */

