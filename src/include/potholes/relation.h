/* 
 * File:   Relation.h
 * Author: sb1708
 *
 * Created on October 7, 2013, 9:49 AM
 */

#ifndef RELATION_H
#define	RELATION_H

namespace potholes { 
    
    class Relation {
    public:
           typedef enum {
                Parent, Child, Equivalent, Replacement, Insertion, Next
                } Type;
        
    };
    
}


#endif	/* RELATION_H */

