/* 
 * File:   ReferenceCountedObject.h
 * Author: sb1708
 *
 * Created on December 18, 2012, 2:59 PM
 */

#ifndef REFERENCECOUNTEDOBJECT_H
#define	REFERENCECOUNTEDOBJECT_H


class ReferenceCountedObject
{
	public: 
		unsigned refCount;
	public:	
	unsigned getRefCount() const {return refCount;};
	static void decrement(ReferenceCountedObject *);
	static void increment(ReferenceCountedObject *);
	ReferenceCountedObject(unsigned);
	virtual ~ReferenceCountedObject();
};



#endif	/* VERILOGREFERENCECOUNTEDOBJECT_H */

