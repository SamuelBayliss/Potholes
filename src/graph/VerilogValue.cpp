#include <graph/VerilogValue.h>

std::string SignedToVerilogConstant(unsigned size, int long value) {
  // assert(size % 4 == 0); // Check simple convert to hex will work. 

  std::stringstream ss;
  // largest 2's complement +ve integer is 2^(n-1) -1

  if (value >= 0)
    {
      //assert(mlog2(value) <= size);
      assert( ((1 << (size-1))-1) >= value); 
      

  if (size % 4 == 0)
    {
      ss << size << "'" << "sh" << std::setw(size / 4) << std::setfill('0') << std::hex << value;
    }
  else
    {
      ss << size << "'" << "sb" ;
      ss << toBinary(size, value);
    }
  
  return ss.str();
    }
  else
    {
      assert (value >= -(1 << (size -1)));
      ss << size << "'" << "sb" ;
      
      ss << toBinary(size, value);
      return ss.str();
    }
}

std::string UnsignedToVerilogConstant(unsigned size, unsigned long value) {
 std::stringstream ss;

 if (value == 1 && size == 0) { 
   return std::string("1'b1");
 }

 if (value == 0 && size == 0) { 
   return std::string("1'b0");
 }

if (value >= 0)
    {
      //assert(mlog2(value) <= size);
      assert( ((1 << (size))-1) >= value); 
      

  if (size % 4 == 0)
    {
      ss << size << "'" << "h" << std::setw(size / 4) << std::setfill('0') << std::hex << value;
    }
  else
    {
      ss << size << "'" << "b" ;
      ss << toBinary(size, value);
    }
  
  return ss.str();
    }
  else
    {
      assert (value >= -(1 << (size -1)));
      ss << size << "'" << "b" ;
      
      ss << toBinary(size, value);
      return ss.str();
    }
}


  std::string toBinary(unsigned size, unsigned value)
 {
      unsigned long tmp = value;
      unsigned long tmp2 = 0; // tmp2 is reversed
      int length = 0;
      std::stringstream ss;
      while (tmp != 0)
      {
        tmp2 = (tmp2<<1) | (tmp&1);
        tmp = tmp>>1;
        ++length;
      }

      //      if (length==0)
      // {
      // ss  << '0';
      // return ss.str();
      //}
      
      while (size>length)
      {
	    ss <<'0';
	    size--;
      }

      while(length>0)
	  {
	    if (size >= length)
	      {
		ss << (tmp2&1)?'1':'0'; 
		--size;
	      }
	    tmp2 = tmp2>>1;
	    --length;
	  
	  }


      

      return ss.str();
    }
std::string UnsignedToVerilogOneHot(unsigned size, unsigned value)
{
  assert(value <= size);
  unsigned one_hot_value = 0x01 << value;
  return SignedToVerilogConstant(size, one_hot_value);

}
