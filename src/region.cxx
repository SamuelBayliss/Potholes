
    const clang::RewriteBuffer& transform() {
        
        /*
          
         Creates Empty Region List 
         
         Call Decompose on tree recursively to get two streams
         
         Call Rewrite on streams to get rewritten buffers
          
         Return rewritten buffer from rewriter
         
         */
        
        
    }
    
      RegionList decompose(RegionList& origin) {
          
          /* This function ensures that the origin list is correctly decomposed 
             
           Call function recursively until you end at a leaf node 
           
            
           * Function also returns a new region list for transformed buffer
           
           */
          
      }
      
      rewrite(clang::Rewriter, RegionList& origin, RegionList & transform) {
          /* 
           Move from one end of the origin list to the other
           * 
           * Check if the current element is identical
           * 
           * if so, move on in both streams
           * 
           * else, check what the destination stream is
           *    if destination stream come from string, insert it and increment transform stream
           * 
           
           
           */
      }