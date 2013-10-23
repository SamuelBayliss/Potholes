/* 
 * File:   Region.h
 * Author: sb1708
 *
 * Created on October 7, 2013, 9:49 AM
 */

#ifndef REGION_H
#define	REGION_H

#include <potholes/relation.h>

#include <list>


namespace potholes {

    class Region;

    class Region {
  
    public:
        typedef std::map<Region *, Relation::Type> RelationMap;
        typedef std::list<Region *> RegionList;
    private:

        RelationMap relations;

    public:
        // Type conversion to string. 

    private:
       RegionList decompose(RegionList&);    
    };

    class EmptyRegion : public Region {
        
    };

    class BufferedRegion : public Region {
        
    };
    
    class FileRegion : public BufferedRegion { 
    public :
        
        // does depth first traversal of buffers calling decompose
        
        // then creates Rewriter object and applies rewrite commands
        const clang::RewriteBuffer& transform(); 

    };
    
    class ASTRegion : public Region { 
        
    };
};

#endif	/* REGION_H */

