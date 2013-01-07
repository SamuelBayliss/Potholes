
#ifdef VERILOG_VALUE_HEADER
#define VERILOG_VALUE_HEADER

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
#endif
