/* 
 * File:   VerilogSymb.h
 * Author: sb1708
 *
 * Created on December 17, 2012, 12:50 PM
 */

#ifndef VERILOGSYMB_H
#define	VERILOGSYMB_H

    class Stack {} ;
    class Heap {}  ;
    
#include <boost/static_assert.hpp>
    
#include <gravel/ReferenceCountedObject.h>
    
namespace Gravel { 

 
     class VerilogValueIf {
        public:
          virtual int getOutput() = 0;
        };

     class VerilogConstantIf : VerilogValueIf {
        public:
         
        };
        
        
template <class T > class VerilogValue_ {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) == 0, "VerilogValue Specified on Heap or Stack");
};


template <> class VerilogValue_<Heap> : public VerilogValueIf, public ReferenceCountedObject { 
  public:
    int getOutput() { 
        return 0;
    }
    
    VerilogValue_() : ReferenceCountedObject(1) // instantiate a reference counted object
    {
        
    
    }
    
};
template <> class VerilogValue_<Stack> : public VerilogValueIf, public ReferenceCountedObject { 

    VerilogValue_<Heap> * shadow;
        public:    
    int getOutput() { 
        return shadow->getOutput();
    }
    VerilogValue_() : ReferenceCountedObject(1) // instantiate a reference counted object
    {
        shadow = new VerilogValue_<Heap>();
    
    }
        
    
};


template <class T> class VerilogConstant_ { 
   BOOST_STATIC_ASSERT_MSG(sizeof(T) == 0, "VerilogConstant Specified on Heap or Stack");
};
template <> class VerilogConstant_<Heap> : public VerilogConstantIf, public VerilogValue_<Heap> { 
  public:
    int getOutput() { 
        return 0;
    }
    
    VerilogValue_() : ReferenceCountedObject(1) // instantiate a reference counted object
    {
        
    
    }
    
};
template <> class VerilogValue_<Stack> : public VerilogValueIf, public ReferenceCountedObject { 

    VerilogValue_<Heap> * shadow;
        public:    
    int getOutput() { 
        return shadow->getOutput();
    }
    VerilogValue_() : ReferenceCountedObject(1) // instantiate a reference counted object
    {
        shadow = new VerilogValue_<Heap>();
    
    }
        
    
};

typedef VerilogValue_<Stack> VerilogValue;

typedef VerilogConstant_<Stack> VerilogConstant;






}
    
#endif	/* VERILOGSYMB_H */

