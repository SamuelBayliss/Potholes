
/*
Expression : Takes several single-bit inputs in an order

VerilogMap vm ;

vm << Symbol << Symbol << Symbol;

vm >> Symbol;

vm(Constant) = Constant;
vm(Constant) = Constant;
vm(Constant) = Constant;

vm("xx1x") = Constant;
*/

//VerilogExpression isn't abstract;

class ActorPtr {
protected : 
  VerilogGraphActor * shadow;
public:
  VerilogGraphNode * getOutput();
}

class ExpressionPtr : public ActorPtr { 

public : 

  VerilogGraphNode * getInput();
}

//VerilogConstants should allow promotion from std::strings, but constructor can raise exception if string can't be parsed correctly

// can call emit on all operations

class VerilogGraphActor {
  // these are operations that only the actor knows about
protected:
  virtual void emit(std::ostream&) = 0;
}

  class VerilogMapSelector : public std::pair<VerilogMap, VerilogConstant> {
    VerilogMapSelector operator=(const VerilogConstant& b); // must register constant in VerilogMap and unregister any conflicting registration
  }


class VerilogMapOperations : VerilogExpression {
  VerilogMapSelector operator()(VerilogConstant); // creates map selector to live on stack
}

Ver

class VerilogMapState  {
  
}


class VerilogMapPtr : public VerilogMapOperations, ExpressionPtr { 
  
  // passes operations through to VerilogMapActor
  
  // This isn't reference counted (these objects always live on the stack)

  // GetOutput implemented from VerilogExpression

}


class VerilogMapActor : public VerilogMapOperations, VerilogMapState { 
  // if a VerilogMapActor is destroyed before it can associate with part of the context, it is added to the "Lost Operators" stack 
  // optionally with a stacktrace
  // users can query the lost operators stack by calling "verify" on context. 
  
} 

typedef VerilogMapPtr VerilogMap;
