/* 
 * File:   register.h
 * Author: sb1708
 *
 * Created on December 13, 2013, 11:03 PM
 */

#include <potholes/analysis.h>

#include "transform.h"

#ifndef REGISTER_H
#define	REGISTER_H
namespace potholes { 

  

template<typename T> std::string Register(std::string name) {
    potholes::Analysis::Registry * reg = potholes::Analysis::Registry::instance();
    TransformFactory<T> * factory = new TransformFactory<T>();
    reg->insert(name, factory);
    return name;
}
};

#endif	/* REGISTER_H */

