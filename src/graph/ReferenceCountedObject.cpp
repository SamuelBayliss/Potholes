/*
 *  ReferenceCountedObject.cpp
 *  VerilogLibrary
 *
 *  Created by Samuel Bayliss on 28/11/2011.
 *  Copyright 2011 Imperial College. All rights reserved.
 *
 */

#include <graph/ReferenceCountedObject.h>
#include <graph/VerilogSymbol.h>

#include <iostream>

void ReferenceCountedObject::decrement(ReferenceCountedObject * obj)
{
  assert(obj);
  assert(obj != 0);

	obj->refCount--;
		std::cout << "Decrement (" << obj->refCount << ")" << "\n";
	
	if (obj->refCount <= 0) {	
	  std::cout << "Decrement causes deallocation" << "\n";
		delete obj;
	}
}
void ReferenceCountedObject::increment(ReferenceCountedObject * obj)
{
  assert(obj);
	obj->refCount++;
	std::cout << "Increment (" << obj->refCount << ")" << "\n";
}

ReferenceCountedObject::~ReferenceCountedObject()
{
	
};

ReferenceCountedObject::ReferenceCountedObject(unsigned count) : refCount(count)
{

}
